//don`t delete this,
//if you delete you can`t send & accept data to server
//your event | accept data from server
void accept_respond(const char * payload, size_t length) {
  Serial.print("Accept Respond : ");
  Serial.println(payload);
}

void accept_cmd(const char * payload, size_t length) {
  DeserializationError error = deserializeJson(in, payload);
  if(!error){
    String input = in["text"];
    execute_data(input);
  }
}
