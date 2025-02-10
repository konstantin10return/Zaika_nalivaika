/* change it with your ssid-password */
const char* ssid = "";
const char* password = "";
/* this is the IP of PC/raspberry where you installed MQTT Server
  on Wins use "ipconfig"
  on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "";

#define MQTT_SERVER_PORT 12110

// пароль mqtt
#define USE_MQTT_PSWD 1
#define MQTT_USER "" 
#define MQTT_PSWD "" 
#define MQTT_DEVICE_ID "MY-ESP32-ВРЕ11-TEST" 

/* topics */
#define MQTT_STATE_TOPIC "user_8202830b/state"
#define MQTT_FROM_CASE_TOPIC "user_8202830b/frome_case"
#define MQTT_TO_CASE_TOPIC "user_8202830b/to_case"

// Пины
#define LED_PIN 2

#define SERVICE_MODE_BTN_PIN 33

#define EMERGENCY_STOP_BTN_PIN 32

const int GLASES_SENSORS_PINS[] = {13, 12, 14, 27};
#define GLASES_SENSORS_PINMODE INPUT_PULLUP
#define GLASS_SENSOR_VALUE_WITHOUT_GLASS 0

// мотор диска
#define LIMIT_SWITCH_PIN 23
#define MOTOR_STEP_PARK_PIN 16
#define MOTOR_DIR_PARK_PIN 4
#define DISK_SPEED 20
#define DISK_AXEL 100

// моторы помп
#define MOTOR_A_STEP_PIN 5
#define MOTOR_A_DIRECTION_PIN 17

#define MOTOR_B_STEP_PIN 19
#define MOTOR_B_DIRECTION_PIN 18

#define MOTOR_C_STEP_PIN 15
#define MOTOR_C_DIRECTION_PIN 21



//серва
//dispenser servo
#define DISPENSER_SERVO_PIN 22

#define SERVO_ROTATION_DELAY 1000




/* ----------------------углы---------------------- */
//диск
// Положения диска для выдачи
const int16_t DISK_STEPPER_POSITIONS_TO_ISSUANCE[] = {-400, -800, -1200, -1500};

//ПОЛОЖЕНИЕ ДИСКА ВО ВРЕМЯ РОЗЛИВА
#define DISK_POSITION_DURING_FILLING 0

// Положение сервы в режиме ожидания
#define SERVO_HOME_VALUE 200

// Положения сервы для розлива
const uint8_t SERVO_VALUES_OVER_GLASES[] = {15, 60, 110, 155};


//Ожидание окончания извлечения наполненого стакана
#define GLASS_REMOVAL_DELAY 2000


// ускорения помп
#define PUMPS_AXEL 10000




//ЭТУ КОНСТАНТУ ЛУЧШЕ НЕ ТРОГАТЬ
#define GLASES_COUNT 4