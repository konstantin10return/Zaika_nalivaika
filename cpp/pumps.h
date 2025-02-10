

uint8_t glase_sensor_pin = (int)GLASES_SENSORS_PINS[0];

#define STOPED_DELAY 200


//
// create the stepper motor object
//

ESP_FlexyStepper stepper_a;
ESP_FlexyStepper stepper_b;
ESP_FlexyStepper stepper_c;





//MOVE_WITH_BREAK
uint16_t move_wiht_break_stepper_a(uint32_t steps){
  //движение
  // при отпускании концевика функция завершается
  stepper_a.setCurrentPositionInSteps(0);
  stepper_a.setTargetPositionInSteps(steps);
  while(!stepper_a.motionComplete()){
    stepper_a.processMovement();

    //Serial.println(String(stepper_a.getCurrentPositionInSteps()));

    check_emergency_stop_btn_in_move();
    if (digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
      if (digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
        steps -= stepper_a.getCurrentPositionInSteps();
        stepper_a.setTargetPositionToStop();
        stepper_a.processMovement();
        delay(STOPED_DELAY);
        Serial.println("Стакан пропал");
        while(digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
          delay(1);
        }
        return(steps);
      }
    }
  }
  return(0);
}


//MOVE_WITH_BREAK
uint16_t move_wiht_break_stepper_b(uint32_t steps){
  //движение
  // при отпускании концевика функция завершается
  stepper_b.setCurrentPositionInSteps(0);
  stepper_b.setTargetPositionInSteps(steps);
  while(!stepper_b.motionComplete()){
    stepper_b.processMovement();
    check_emergency_stop_btn_in_move();
    if (digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
      if (digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
        steps -= stepper_b.getCurrentPositionInSteps();
        stepper_b.setTargetPositionToStop();
        stepper_b.processMovement();
        delay(STOPED_DELAY);
        Serial.println("Стакан пропал");
        while(digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
          delay(1);
        }
        return(steps);
      }
    }
  }
  return(0);
}


//MOVE_WITH_BREAK
uint16_t move_wiht_break_stepper_c(uint32_t steps){
  //движение
  // при отпускании концевика функция завершается
  stepper_c.setCurrentPositionInSteps(0);
  stepper_c.setTargetPositionInSteps(steps);
  while(!stepper_c.motionComplete()){
    stepper_c.processMovement();
    check_emergency_stop_btn_in_move();
    if (digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
      if (digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
        steps -= stepper_c.getCurrentPositionInSteps();
        stepper_c.setTargetPositionToStop();
        stepper_c.processMovement();
        delay(STOPED_DELAY);
        Serial.println("Стакан пропал");
        while(digitalRead(glase_sensor_pin) == GLASS_SENSOR_VALUE_WITHOUT_GLASS){
          delay(1);
        }
        return(steps);
      }
    }
  }
  return(0);
}


void move_stepper_a(uint32_t steps){
  while(steps != 0){
    steps = move_wiht_break_stepper_a(steps);
  }
}


void move_stepper_b(uint32_t steps){
  while(steps != 0){
    steps = move_wiht_break_stepper_b(steps);
  }
}


void move_stepper_c(uint32_t steps){
  while(steps != 0){
    steps = move_wiht_break_stepper_c(steps);
  }
}