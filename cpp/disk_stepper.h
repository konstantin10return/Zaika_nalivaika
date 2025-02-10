bool abc;

ESP_FlexyStepper disk_stepper;


void park_disk_stepper(){
  Serial.println("парковка");
  disk_stepper.setStepsPerMillimeter(25 * 1);
  // set the speed and acceleration rates for the stepper motor
  //
  disk_stepper.setSpeedInMillimetersPerSecond(10.0);
  disk_stepper.setAccelerationInMillimetersPerSecondPerSecond(10.0);
  //
  // move the motor toward the limit switch to find the "Home" position
  //
  const float homingSpeedInMMPerSec = 5.0;
  const float maxHomingDistanceInMM = 380;   // since my lead-screw is 38cm long, should never move more than that
  const int directionTowardHome = -1;        // direction to move toward limit switch: 1 goes positive direction, -1 backward
  
  if(disk_stepper.moveToHomeInMillimeters(directionTowardHome, homingSpeedInMMPerSec, maxHomingDistanceInMM, LIMIT_SWITCH_PIN) != true)
  {
    //
    // this code is executed only if homing fails because it has moved farther 
    // than maxHomingDistanceInMM and never finds the limit switch, blink the 
    // LED fast forever indicating a problem
    //
    Serial.println("ОШИБКА ПАРКОВКИ");
    while(true)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
      delay(50);
    }
  }
  Serial.println('Успешная парковка');
}