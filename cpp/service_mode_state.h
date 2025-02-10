void on_nessage_in_service_dode_state(String mesage){
  if(mesage == "exit"){
    state = WAITING_ST;
    send_state();
  }
}