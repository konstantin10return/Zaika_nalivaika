from paho.mqtt import client as mqtt_client

import time

import settings


MQTT_HOST = 'm1.wqtt.ru'
MQTT_PORT = 12110
MQTT_CLIENT_ID = 'fjnbnroitb'

USE_PSWD = True
MQTT_USER = 'u_GTRZWP'
MQTT_PSWD = 'nKeOD18W'

# MQTT_PREFIX = 'user_8202830b/'
MQTT_STATE_TOPIC = "user_8202830b/state"
MQTT_FROM_CASE_TOPIC = 'user_8202830b/frome_case'
MQTT_TO_CASE_TOPIC = 'user_8202830b/to_case'


class MqttHelper:
    def __init__(self):
        self.client = mqtt_client.Client()
        if USE_PSWD:
            self.client.username_pw_set(MQTT_USER, MQTT_PSWD)
        self.client.on_connect = self.on_connect
        self.client.connect(MQTT_HOST, MQTT_PORT)
        self.client.tls_set()
        self.service_mode = False
        self.service_mode_mesages = []

    def start(self):
        self.client.loop_forever()

    def turn_off_service_mode(self):
        self.service_mode = False
        self.service_mode_mesages.clear()

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print(f"Connected to '{MQTT_HOST}' host")
            topics = [(MQTT_FROM_CASE_TOPIC, 0), (MQTT_STATE_TOPIC, 0)]
            self.client.subscribe(topics)
            self.client.on_message = self.on_message
            self.get_state()
        else:
            print(f'Failed to connect, return code {rc}')
            raise 'подключиться не удалось'

    def on_message(self, client, userdata, msg):
        print(f"Received '{str(msg.payload)}' from '{msg.topic}' topic")
        if msg.topic == MQTT_STATE_TOPIC:
            print(msg.payload.decode())
            self.chainge_state(msg.payload.decode())
        elif msg.topic == MQTT_FROM_CASE_TOPIC:
            if self.service_mode:
                self.service_mode_mesages.append(msg.payload.decode())

    def send_order(self, order):
        self.client.publish(MQTT_TO_CASE_TOPIC, self.reformat_order(order))

    def get_state(self):
        self.client.publish(MQTT_TO_CASE_TOPIC, 'get_state')

    def send_command_in_service_mode(self, command):
        self.client.publish(MQTT_TO_CASE_TOPIC, command.strip())

    def reformat_order(self, order):
        reformated_order = str(settings.order_prefix)
        reformated_order += ''.join([str(settings.drinks_and_message_coeds[i]) for i in order])
        reformated_order += str(settings.empty_glass_value) * (settings.max_glases_count - len(order))
        return reformated_order

    def close(self):
        print('mqtt_stop ')
        self.client.loop_stop()
