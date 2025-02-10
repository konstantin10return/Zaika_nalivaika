from PySide6.QtWidgets import QMainWindow
from PySide6.QtCore import QThread
from PySide6.QtCore import Signal as pyqtSignal
import time

from cooking_ui import Ui_MainWindow

import settings


CHECK_STATE_TIMEOUT = 1

ex = None


class Worker(QThread):
    # Сигнал для передачи данных из рабочего потока в основной поток
    reload_signal = pyqtSignal()

    def run(self):
        global CHECK_STATE_TIMEOUT
        time.sleep(1)
        while True:
            if ex.get_state() != 'cooking':
                self.reload_signal.emit()
                return None
            time.sleep(CHECK_STATE_TIMEOUT)


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, callbeck, get_state):
        print('cooking_init')
        self.get_state = get_state
        super().__init__()
        self.callbeck = callbeck
        self.setupUi(self)
        self.thread = Worker()
        self.thread.reload_signal.connect(self.reload_if_state_changed)
        self.thread.start()

    def resize(self, *args):
        if settings.RESIZE_INTERFEISE:
            super().resize(*args)

    def reload_if_state_changed(self):
        print('change_state')
        self.callbeck()



def load(callbeck, get_state):
    global ex
    ex = MainWindow(callbeck, get_state)
    ex.show()
    return ex