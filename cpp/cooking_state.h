// переменные для приготовления
//int16_t order[GLASES_COUNT];
//int16_t glass_namber = 0;


void finish_cooking(){
  //analogWrite(DISPENSER_SERVO_PIN, SERVO_HOME_VALUE);
  dispenser_servo.write(SERVO_HOME_VALUE);
  state = ISSUANCE_ST;
  //send_state();
  start_issuing();
}


void cooking_handler(){
  if(glass_namber >= GLASES_COUNT){
    finish_cooking();
  } else if(order[glass_namber] == 0){
    finish_cooking();
  } else{
    dispenser_servo.write(SERVO_VALUES_OVER_GLASES[glass_namber]);
    delay(SERVO_ROTATION_DELAY);
    glase_sensor_pin = (int)GLASES_SENSORS_PINS[glass_namber];
    cooking_drink(order[glass_namber]);
    prepared_drinks_count++;
  }
  glass_namber++;
  
}