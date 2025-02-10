#define TASK_CHECK_SERVICE_MODE_BTN_TIMEOUT 250



void waiting_handler(){
  static uint64_t task_check_service_mode_btn_lasttime = 0;
  static uint64_t now;
  now = millis();
  if (fabs(now - task_check_service_mode_btn_lasttime) >= TASK_CHECK_SERVICE_MODE_BTN_TIMEOUT){
    if(digitalRead(SERVICE_MODE_BTN_PIN) + digitalRead(SERVICE_MODE_BTN_PIN) == 0){
      state = SERVICE_MODE_ST;
      send_state();
    }
    task_check_service_mode_btn_lasttime = now;
  }
}


/*void registrate_order(){
  uint8_t j = 0;
  for(uint8_t i = 6; i < length; i++){
      order[j] = payload[i] - 48;
      Serial.print(String(order[j]));
  }
  Serial.println();
}*/


void on_nessage_in_waiting_state(String mesage){
  Serial.println(mesage.substring(0, 6));
  Serial.println("сообщение в состоянии ожидания");

  if(mesage.substring(0, 6) == "order:"){
    Serial.println("получен заказ:");
    // registrate_order();
    uint8_t j = 0;
    String order_str = mesage.substring(6, GLASES_COUNT + 6);
    Serial.println(order_str);
    uint8_t i = 0;
    for(uint8_t i = 0; i < GLASES_COUNT; i++){
      order[i] = char(order_str[i]) - 48;
      Serial.print(String(order[i]));
    }
    Serial.println();
    glass_namber = 0;
    prepared_drinks_count = 0;
    state = COOKING_ST;
    send_state();
  }
}