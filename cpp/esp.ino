#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP_FlexyStepper.h>
#include <ESP32Servo.h> 


#include<configuration.h>


#define TASK_CHECK_EMERGENCY_STOP_BTN_TIMEOUT 200


uint64_t task_check_emergency_stop_btn_lasttime = 0;

Servo dispenser_servo;


void task_check_emergency_stop_btn_handler(){
  if(digitalRead(EMERGENCY_STOP_BTN_PIN) + digitalRead(EMERGENCY_STOP_BTN_PIN) == 0){
    Serial.println('АВАРИЙНАЯ ОСТАНОВКА');
    while(true)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
      delay(50);
    }
  }
}


void check_emergency_stop_btn_in_move(){
  static uint64_t now;
  now = millis();
  if (fabs(now - task_check_emergency_stop_btn_lasttime) >= TASK_CHECK_EMERGENCY_STOP_BTN_TIMEOUT){
    task_check_emergency_stop_btn_handler();
    task_check_emergency_stop_btn_lasttime = now;
  }
}


#include<disk_stepper.h>
#include<pumps.h>
#include<recipes.h>


// переменные для приготовления и выдачи
int16_t order[GLASES_COUNT];
int16_t glass_namber = 0;
// переменная для выдачи
uint8_t prepared_drinks_count = 0;



#define SETUP_ST 0
#define WAITING_ST 1
#define SERVICE_MODE_ST 2
#define COOKING_ST 3
#define ISSUANCE_ST 4


uint8_t state = SETUP_ST;

String mesage = "";

WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE  (50)
//char msg[MSG_BUFFER_SIZE];


void send_state(){
  Serial.println("state = " + String(state));
  char msg[MSG_BUFFER_SIZE];
  if(state == SETUP_ST){
    msg[0] = 's';
    msg[1] = 'e';
    msg[2] = 't';
    msg[3] = 'u';
    msg[4] = 'p';
  }
  if(state == WAITING_ST){
    msg[0] = 'w';
    msg[1] = 'a';
    msg[2] = 'i';
    msg[3] = 't';
    msg[4] = 'i';
    msg[5] = 'n';
    msg[6] = 'g';
  }
  if(state == SERVICE_MODE_ST){
    msg[0] = 's';
    msg[1] = 'e';
    msg[2] = 'r';
    msg[3] = 'v';
    msg[4] = 'i';
    msg[5] = 'c';
    msg[6] = 'e';
    msg[7] = '_';
    msg[8] = 'm';
    msg[9] = 'o';
    msg[10] = 'd';
    msg[11] = 'e';
  }
  if(state == COOKING_ST){
    msg[0] = 'c';
    msg[1] = 'o';
    msg[2] = 'o';
    msg[3] = 'k';
    msg[4] = 'i';
    msg[5] = 'n';
    msg[6] = 'g';
  }
  if(state == ISSUANCE_ST){
    msg[0] = 'i';
    msg[1] = 's';
    msg[2] = 's';
    msg[3] = 'u';
    msg[4] = 'a';
    msg[5] = 'n';
    msg[6] = 'c';
    msg[7] = 'e';
  }
  client.publish(MQTT_STATE_TOPIC, msg);
}


#include <waiting_state.h>
#include <setup_state.h>
#include<issuance_state.h>
#include <cooking_state.h>
#include <service_mode_state.h>


void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Command received");
  Serial.println(topic);

  /*Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  */
  Serial.println();

  String mesage = "";
  for (int i = 0; i < length; i++) {
      mesage +=(char)payload[i];
  }

  Serial.println(mesage);

  digitalWrite(LED_PIN, 0);
  delay(100);
  digitalWrite(LED_PIN, 1);
  delay(1000);


  if(mesage == "get_state"){
    //Serial.println(String(mesage) + " == get_state");
    send_state();
  } else if(state == WAITING_ST){
    on_nessage_in_waiting_state(mesage);
  } else if(state == SERVICE_MODE_ST){
    on_nessage_in_service_dode_state(mesage);
  }
}


bool attempted_mqtt_connection(){
  if(USE_MQTT_PSWD == 0){
    String clientId = "ESP32Client";
    return client.connect(clientId.c_str());
  } else{
    return client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PSWD);
  }
}


void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP32Client";
    /* connect now */
    if (attempted_mqtt_connection()) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(MQTT_TO_CASE_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 2 seconds");
      /* Wait 5 seconds before retrying */
      delay(2000);
    }
  }
  digitalWrite(LED_PIN, 1);
}


void setup(){
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  dispenser_servo.setPeriodHertz(50); // Standard 50hz servo
  dispenser_servo.attach(DISPENSER_SERVO_PIN, 500, 2400);
  dispenser_servo.write(SERVO_HOME_VALUE);

  uint8_t initialized_glas_sensor_pin;
  for(uint8_t i = 0; i < GLASES_COUNT; i++){
    initialized_glas_sensor_pin = (int)GLASES_SENSORS_PINS[i];
    pinMode(initialized_glas_sensor_pin, GLASES_SENSORS_PINMODE);
  }

  pinMode(LIMIT_SWITCH_PIN, INPUT);
  pinMode(EMERGENCY_STOP_BTN_PIN, INPUT);
  pinMode(SERVICE_MODE_BTN_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  //pinMode(DISPENSER_SERVO_PIN, OUTPUT);
  //analogWrite(DISPENSER_SERVO_PIN, SERVO_HOME_VALUE);
  
  // connect and configure the stepper motor to its IO pins
  stepper_a.connectToPins(MOTOR_A_STEP_PIN, MOTOR_A_DIRECTION_PIN);
  stepper_b.connectToPins(MOTOR_B_STEP_PIN, MOTOR_B_DIRECTION_PIN);
  stepper_c.connectToPins(MOTOR_C_STEP_PIN, MOTOR_C_DIRECTION_PIN);
  disk_stepper.connectToPins(MOTOR_STEP_PARK_PIN, MOTOR_DIR_PARK_PIN);

  stepper_a.setAccelerationInStepsPerSecondPerSecond(PUMPS_AXEL);
  stepper_b.setAccelerationInStepsPerSecondPerSecond(PUMPS_AXEL);
  stepper_c.setAccelerationInStepsPerSecondPerSecond(PUMPS_AXEL);

  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  /* set led as output to control led on-off */
  //pinMode(led, OUTPUT);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, MQTT_SERVER_PORT);
  /* this receivedCallback function will be invoked
    when client received subscribed topic */
  
  client.setCallback(receivedCallback);
}


void loop() {
  static uint64_t now;
  now = millis();
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming
    subscribed topic-process-invoke receivedCallback */
  client.loop();

  if(state == SETUP_ST){
    setup_handler();
  }
  if(state == WAITING_ST){
    waiting_handler();
  }
  if(state == COOKING_ST){
    cooking_handler();
  }
  if(state == ISSUANCE_ST){
    issuance_handler();
  }

  if (fabs(now - task_check_emergency_stop_btn_lasttime) >= TASK_CHECK_EMERGENCY_STOP_BTN_TIMEOUT){
    task_check_emergency_stop_btn_handler();
    task_check_emergency_stop_btn_lasttime = now;
  }
}

