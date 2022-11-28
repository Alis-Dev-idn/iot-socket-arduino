#include <ArduinoJson.h>
#include <WiFi.h>
#include <SocketIoClient.h>

StaticJsonDocument<200> doc;

/// WIFI Settings ///
const char* ssid     = "SMART XIRKA";
const char* password = "EngInEEr@30";

/// Socket.IO Settings ///
//char host[] = "db.smpvanilla.com";
char host[] = "api.smpvanilla.com";
int port = 5042;
char path[] = "/socket.io/?transport=websocket";
bool useSSL = true;
const char * sslFingerprint = "";
const char* topic = "send_data";

unsigned long lastTime = 0;

SocketIoClient webSocket;
WiFiClient client;

void socket_Connected(const char * payload, size_t length) {
  Serial.println("Socket.IO Connected!");
}

void socket_event(const char * payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  webSocket.on(topic, socket_event);
  
  if (useSSL) {
    webSocket.beginSSL(host, port, path, sslFingerprint);
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
  doc["name"] = "node_1";
  serializeJson(doc, json);
  webSocket.emit(topic, json);
}
