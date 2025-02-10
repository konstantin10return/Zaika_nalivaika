// здесь задаются рецепты

/*a. Газированная вода (50 мл.)
b. Мятный сироп (10 мл.)
c. Апельсиновый сок (40 мл.)
d. Лимонад “Мятный” (80 мл. газированной воды + 20 мл. мятного сиропа).
e. Лимонад “Заводной апельсин” (30 мл. газированной воды + 50 мл. апельсинового сока).
f. Лимонад ‘Тройной” (35 мл. газированной воды + 45 мл. апельсинового сока + 10 мл. мятного сиропа) */

void cooking_drink(int16_t recipe_id){
  if(recipe_id == 1){
    Serial.println("Газированная вода");
    stepper_a.setSpeedInStepsPerSecond(50000);
    move_stepper_a(5000);
    stepper_a.setSpeedInStepsPerSecond(1000);
    stepper_a.moveRelativeInSteps(-10);
    }
  if(recipe_id == 2){
    Serial.println("Мятный сироп");
    stepper_b.setSpeedInStepsPerSecond(50000);
    move_stepper_b(1000);
    stepper_b.setSpeedInStepsPerSecond(1000);
    stepper_b.moveRelativeInSteps(-10);
  }
  if(recipe_id == 3){
    Serial.println("Апельсиновый сок");
    stepper_c.setSpeedInStepsPerSecond(50000);
    move_stepper_c(5000);
    stepper_c.setSpeedInStepsPerSecond(1000);
    stepper_c.moveRelativeInSteps(-10);
  }
  if(recipe_id == 4){
    Serial.println("Лимонад <<Мятный>>");
    stepper_a.setSpeedInStepsPerSecond(50000);
    move_stepper_a(8000);
    stepper_a.setSpeedInStepsPerSecond(1000);
    stepper_a.moveRelativeInSteps(-10);

    stepper_b.setSpeedInStepsPerSecond(50000);
    move_stepper_b(2000);
    stepper_b.setSpeedInStepsPerSecond(1000);
    stepper_b.moveRelativeInSteps(-10);
  }
  if(recipe_id == 5){
    Serial.println("Лимонад <<Заводной апельсин>>");
    stepper_a.setSpeedInStepsPerSecond(5000);
    move_stepper_a(3000);
    stepper_a.setSpeedInStepsPerSecond(1000);
    stepper_a.moveRelativeInSteps(-10);

    stepper_c.setSpeedInStepsPerSecond(50000);
    move_stepper_c(5000);
    stepper_c.setSpeedInStepsPerSecond(1000);
    stepper_c.moveRelativeInSteps(-10);
  }
  if(recipe_id == 6){
    Serial.println("Лимонад <<Тройной>>");
    stepper_a.setSpeedInStepsPerSecond(50000);
    move_stepper_a(3500);
    stepper_a.setSpeedInStepsPerSecond(1000);
    stepper_a.moveRelativeInSteps(-10);

    stepper_b.setSpeedInStepsPerSecond(50000);
    move_stepper_b(4500);
    stepper_b.setSpeedInStepsPerSecond(1000);
    stepper_b.moveRelativeInSteps(-10);

    stepper_c.setSpeedInStepsPerSecond(50000);
    move_stepper_c(1000);
    stepper_c.setSpeedInStepsPerSecond(1000);
    stepper_c.moveRelativeInSteps(-10);
  }
}