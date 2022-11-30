//don`t delete this,
//if you delete you can`t send & accept data to server
//your event | accept data from server
void socket_accept(const char * payload, size_t length) {
  Serial.print("Accept Message : ");
  Serial.println(payload);
}