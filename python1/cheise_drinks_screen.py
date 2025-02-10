import time
import sys
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtWidgets import QScrollArea, QWidget, QVBoxLayout, QGroupBox, QLabel
from PySide6.QtWidgets import QPushButton, QHBoxLayout
from PySide6.QtWidgets import QInputDialog
from PySide6.QtWidgets import QMessageBox
from PySide6.QtCore import QThread
from PySide6.QtCore import Signal as pyqtSignal

from cheise_drinks_ui import Ui_MainWindow

import settings

CHECK_STATE_TIMEOUT = 3


ex = None
order = None
max_id = 0


class Worker(QThread):
    # Сигнал для передачи данных из рабочего потока в основной поток
    reload_signal = pyqtSignal()

    def run(self):
        global CHECK_STATE_TIMEOUT
        time.sleep(1)
        while True:
            if ex.get_state() != 'waiting':
                self.reload_signal.emit()
                return None
            time.sleep(CHECK_STATE_TIMEOUT)


class DrinkGroupBox(QGroupBox):
    def __init__(self, drink_name):
        global max_id
        super().__init__('')
        max_id += 1
        self.id = max_id

        # Создаем вертикальный макет для содержимого GroupBox
        self.layout = QVBoxLayout()

        # Создаем горизонтальный макет для кнопок
        button_layout = QHBoxLayout()

        # Добавляем маленькую кнопку
        small_button = QPushButton("Удалить")
        small_button.setFixedSize(100, 30)  # Задаем фиксированный размер
        button_layout.addWidget(small_button)
        small_button.clicked.connect(self.selfdestruction)

        # Добавляем большую кнопку
        self.drink_name = drink_name
        self.large_button = QPushButton(drink_name)
        self.large_button.setFixedSize(350, 50)  # Задаем фиксированный размер
        button_layout.addWidget(self.large_button)
        self.large_button.clicked.connect(self.change_drink)

        # Добавляем горизонтальный макет с кнопками в групповой макет
        self.layout.addLayout(button_layout)

        # Устанавливаем макет для GroupBox
        self.setLayout(self.layout)

        # Устанавливаем фиксированный размер для GroupBox
        self.setFixedHeight(100)

    def change_drink(self):
        drink_name, ok_pressed = QInputDialog.getItem(self, "Выберите напиток", "", settings.drinks, 1, False)
        if ok_pressed:
            print(drink_name)
            self.drink_name = drink_name
            self.large_button.setText(drink_name)

    def selfdestruction(self):
        ex.glasses_count -= 1
        ex.btn_add_glass.setDisabled(False)
        ex.glasses_set.discard(self)
        if ex.glasses_count <= 0:
            ex.btn_send.setDisabled(True)
        ex.scroll_layout.removeWidget(self)
        self.deleteLater()


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, callbeck, get_state):
        super().__init__()
        print('init cheise_drinks')
        self.setupUi(self)
        self.btn_send.setDisabled(True)
        print(self.btn_send.text())
        self.csllbeck = callbeck
        self.btn_add_glass.clicked.connect(self.add_drink)
        self.scroll_layout = QVBoxLayout(self.scrollAreaWidgetContents)
        self.label.setText(f'Выберете напитки (до {settings.max_glases_count} стаканов)')

        self.btn_send.clicked.connect(self.order_registration)

        self.glasses_count = 0
        self.glasses_set = set()

        self.get_state = get_state
        self.callbeck = callbeck

        self.centralwidget.setLayout(self.verticalLayout)

        self.btn_send.setFixedHeight(80)
        self.btn_add_glass.setFixedHeight(50)

        self.thread = Worker()
        self.thread.reload_signal.connect(self.reload_if_state_changed)
        print('init cheise_drinks')
        self.thread.start()

    def resize(self, *args):
        if settings.RESIZE_INTERFEISE:
            super().resize(*args)

    def reload_if_state_changed(self):
        self.callbeck(None)

    def add_drink(self, *args, **kwargs):
        print('add_drink')
        drink_name, ok_pressed = QInputDialog.getItem(self, "Выберите напиток", "", settings.drinks, 1, False)
        if ok_pressed:
            print(drink_name)
            group_box = DrinkGroupBox(drink_name)
            self.glasses_set.add(group_box)
            self.scroll_layout.addWidget(group_box)
            self.glasses_count += 1
            self.btn_send.setDisabled(False)
        if self.glasses_count >= settings.max_glases_count:
            self.btn_add_glass.setDisabled(True)

    def order_registration(self):
        global order
        wont_to_sande = str(QMessageBox.question(self, 'Подтверждение заказа', 'Отправить заказ на выполнение ?'))
        if 'StandardButton.Yes' == wont_to_sande or str(wont_to_sande) == "16384":
            order = [i.drink_name for i in sorted(list(self.glasses_set), key=lambda x: x.id)]
            print(order)
            self.csllbeck()


def load(callbeck, get_state):
    global ex
    ex = MainWindow(callbeck=callbeck, get_state=get_state)
    ex.show()
    return ex
