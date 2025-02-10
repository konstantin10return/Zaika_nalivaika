

void start_issuing(){
  if(prepared_drinks_count > 0){
    glass_namber = 0;
    disk_stepper.moveToPositionInSteps(DISK_STEPPER_POSITIONS_TO_ISSUANCE[glass_namber]);
  }
}


void finish_ssuance(){
  prepared_drinks_count = 0;
  glass_namber = 0;
  for(uint8_t i; i < GLASES_COUNT; i++){
    order[i] = 0;
  }

  disk_stepper.moveToPositionInSteps(DISK_POSITION_DURING_FILLING);

  state = WAITING_ST;
  send_state();
}


void issuance_handler(){
  Serial.println(String(digitalRead(GLASES_SENSORS_PINS[glass_namber - 1])) + " " + String(glass_namber - 1));
  if(GLASS_SENSOR_VALUE_WITHOUT_GLASS == digitalRead(GLASES_SENSORS_PINS[glass_namber - 1])){
    delay(GLASS_REMOVAL_DELAY);
    Serial.println("выдан стакан" + String(glass_namber));
    glass_namber++;
    if(glass_namber > prepared_drinks_count){
      finish_ssuance();
    } else if(glass_namber > GLASES_COUNT){
      finish_ssuance();
    } else{
      disk_stepper.moveToPositionInSteps(DISK_STEPPER_POSITIONS_TO_ISSUANCE[glass_namber - 1]);
    }
  }
}