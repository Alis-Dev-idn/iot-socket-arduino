#include <ArduinoJson.h>
#include <WiFi.h>
#include "./src/library/SocketIoClient/SocketIoClient.h"

#define led 2

StaticJsonDocument<200> doc;
StaticJsonDocument<200> in;
StaticJsonDocument<200> out;

/// WIFI Settings ///
const char* ssid     = "YOUR SSID WIFI";
const char* password = "YOUR PASSWORD WIFI";

/// Socket.IO Settings ///
char host[] = "api.smpvanilla.com";
int port = 5042;
char path[] = "/socket.io/?transport=websocket";
bool useSSL = true;
const char* topic_server = "send_data"; //don`t delete this, you can`t send data if modify or delete
const char* device = "YOUR DEVICE NAME"; //name your device if empty can`t accept data from command
const char* key = "YOUR KEY DEVICE"; //key yout device from page device you create

unsigned long lastTime = 0;

SocketIoClient webSocket;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //don`t delete or modify this, if you modify you can`t accpet data from server
  webSocket.on(topic_server , accept_respond);
  webSocket.on(device , accept_cmd);
  
  if (useSSL) {
    webSocket.beginSSL(host, port, path, "");
  } else {
    webSocket.begin(host, port, path);
  }
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  webSocket.loop();
  if(millis() - lastTime > 2000){
    generate_json();
    lastTime = millis();
  }
}

void generate_json(){
  char json[256];
  doc["data"]["suhu"] = random(0, 200);
  doc["data"]["humidity"] = random(0, 110);
  doc["data"]["flow"] = random(0, 100);
  doc["data"]["oksigen"] = random(0, 100); 
  doc["key"] = key;
  serializeJson(doc, json);
  webSocket.emit(topic_server, json);
}

void execute_data(String value) {
  char result[128];
  if(value == "on"){
      create_reply_data("led on");
      digitalWrite(led, HIGH);
  }
  if(value == "off"){
      create_reply_data("led off");
      digitalWrite(led, LOW);
  }
}

void create_reply_data(const char* value) {
  char reply_data[256];
  out["data"]["reply_data"] = value;
  out["key"] = key;
  serializeJson(out, reply_data);
  webSocket.emit(topic_server, reply_data);
}
