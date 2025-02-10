from PySide6.QtWidgets import QMainWindow
from PySide6.QtCore import QThread
from PySide6.QtCore import Signal as pyqtSignal
import time

from service_mode_ui import Ui_MainWindow

import settings


CONSOLE_COMMAND_PREFIX = 'root@Лимонадный_автомат:~# '
EXIT_COMAND = 'exit'

CHECK_STATE_AND_MQTT_MESEGES_TIMEOUT = 0.5

ex = None
mqtt_helper = None


class Worker(QThread):
    # Сигнал для передачи данных из рабочего потока в основной поток
    reload_signal = pyqtSignal()
    update_text_browset_signal = pyqtSignal(str)

    def run(self):
        global CHECK_STATE_TIMEOUT
        time.sleep(1)
        while True:
            if ex.get_state() != 'service_mode':
                self.reload_signal.emit()
                return None
            self.check_mqtt_meseges()
            time.sleep(CHECK_STATE_AND_MQTT_MESEGES_TIMEOUT)

    def check_mqtt_meseges(self):
        if len(mqtt_helper.service_mode_mesages) > 0:
            self.update_text_browset_signal.emit('\n'.join([mqtt_helper.service_mode_mesages.pop(0) for _ in range(len(mqtt_helper.service_mode_mesages))]))


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, callbeck, get_state):
        print('cooking_init')
        self.get_state = get_state
        super().__init__()
        self.callbeck = callbeck
        self.setupUi(self)
        self.thread = Worker()
        self.thread.reload_signal.connect(self.reload_if_state_changed)
        self.thread.update_text_browset_signal.connect(self.update_text)
        self.thread.start()

        self.btn_send_comand.clicked.connect(self.send_command)
        self.btn_beck.clicked.connect(self.send_exit_comand)
        self.centralwidget.setLayout(self.verticalLayout)

    def resize(self, *args):
        if settings.RESIZE_INTERFEISE:
            super().resize(*args)

    def reload_if_state_changed(self):
        print('change_state')
        self.callbeck(None)

    def update_text(self, s):
        self.console.append(s)

    def send_command(self):
        comand = self.command_line.text()
        self.command_line.setText('')
        self.console.append(CONSOLE_COMMAND_PREFIX + comand)
        mqtt_helper.send_command_in_service_mode(comand)

    def send_exit_comand(self):
        self.console.append(CONSOLE_COMMAND_PREFIX + EXIT_COMAND)
        mqtt_helper.send_command_in_service_mode(EXIT_COMAND)


def load(callbeck, get_state):
    global ex
    ex = MainWindow(callbeck, get_state)
    ex.show()
    return ex