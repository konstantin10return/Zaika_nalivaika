// void on_nessage_in_setup_state(char* topic, byte* payload, unsigned int length){}


void setup_handler(){
  static uint8_t setup_step = 0;
  if(setup_step == 0){
    park_disk_stepper();
    disk_stepper.setSpeedInMillimetersPerSecond(DISK_SPEED);
    disk_stepper.setAccelerationInStepsPerSecondPerSecond(DISK_AXEL);
  }
  if(setup_step == 1){
    disk_stepper.moveToPositionInSteps(DISK_POSITION_DURING_FILLING);
  }
  if(setup_step == 2){
    state = WAITING_ST;
    send_state();
  }
  setup_step ++;
  Serial.println(setup_step);
}