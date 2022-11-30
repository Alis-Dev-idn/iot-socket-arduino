#include <ArduinoJson.h>
#include <WiFi.h>
#include "./src/library/SocketIoClient/SocketIoClient.h"

StaticJsonDocument<200> doc;

/// WIFI Settings ///
const char* ssid     = "YOUR SSID WIFI";
const char* password = "YOUR PASSWORD WIFI";

/// Socket.IO Settings ///
char host[] = "api.smpvanilla.com";
int port = 5042;
char path[] = "/socket.io/?transport=websocket";
bool useSSL = true;
const char* topic_server = "send_data"; //don`t delete this, you can`t send data if modify or delete
const char* key = "YOUR KEY DEVICE"; //from page device you create

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
  webSocket.on(key , socket_accept);
  
  if (useSSL) {
    webSocket.beginSSL(host, port, path, "");
  } else {
    webSocket.begin(host, port, path);
  }
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
