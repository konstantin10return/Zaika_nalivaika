import sys

import time

from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QThread
from PySide6.QtCore import Signal as pyqtSignal

import cheise_drinks_screen
import cooking_screen
import service_mode_screen
import issuance_screen
import setup_screen

import mqtt


ALL_STATES = ['waiting', 'cooking', 'service_mode', 'issuance', 'setup']

state = 'setup'
ex = None
close_window_before_load = False
block_close = False
mqtt_tread = None


def load():
    global app
    global ex, close_window_before_load
    global state
    global block_close
    global mqtt_tread
    if state == 'waiting':
        new_ex = cheise_drinks_screen.load(cheise_drinks_screen_colbeck, get_state)
    elif state == 'cooking':
        new_ex = cooking_screen.load(cooking_screen_callbeck, get_state)
    elif state == 'service_mode':
        new_ex = service_mode_screen.load(get_state=get_state, callbeck=service_mode_callbeck)
        mqtt_helper.service_mode = True
    elif state == 'issuance':
        new_ex = issuance_screen.load(issuance_screen_callbeck, get_state=get_state)
    elif state == 'setup':
        new_ex = setup_screen.load(setup_screen_callbeck, get_state=get_state)
    else:
        print(state)
        raise "state"
    if close_window_before_load:
        block_close = True
        ex.close()
        block_close = False
    else:
        close_window_before_load = True
    ex = new_ex
    ex.closeEvent = close
    if state != 'service_mode' and mqtt_helper.service_mode:
        mqtt_helper.turn_off_service_mode()


def cheise_drinks_screen_colbeck(new_state='cooking'):
    global state
    if new_state is None:
        load()
        return None
    state = new_state
    mqtt_helper.send_order(cheise_drinks_screen.order)
    print(state)
    load()


def cooking_screen_callbeck(new_state=None):
    global state
    if new_state is None:
        load()
        return None
    state = new_state
    load()


def issuance_screen_callbeck(new_state=None):
    global state
    if new_state is None:
        load()
        return None
    state = new_state
    load()


def setup_screen_callbeck(new_state=None):
    global state
    if new_state is None:
        load()
        return None
    state = new_state
    load()


def service_mode_callbeck(new_state='waiting'):
    global state
    if new_state is None:
        load()
        return None
    state = new_state
    load()


def close(*args, **kwargs):
    global block_close
    if not block_close:
        print('close_all')
        mqtt_helper.close()


def chainge_state_by_mqtt(new_state):
    global state
    if new_state != state:
        for i in ALL_STATES:
            if i in new_state:
                state = i
        # state = new_state
        print(state)


def get_state():
    global state
    return state


def setup_mqtt_tread():
    global mqtt_tread
    mqtt_helper.chainge_state = chainge_state_by_mqtt
    mqtt_tread = QThread()
    mqtt_tread.run = mqtt_helper.start
    mqtt_tread.reload_signal = pyqtSignal()
    mqtt_tread.start()


if __name__ == "__main__":
    mqtt_helper = mqtt.MqttHelper()
    setup_mqtt_tread()
    service_mode_screen.mqtt_helper = mqtt_helper

    app = QApplication(sys.argv)
    load()
    sys.exit(app.exec())