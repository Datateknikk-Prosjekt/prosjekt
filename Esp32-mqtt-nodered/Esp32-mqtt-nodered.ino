

//lokale bibliotek
#include "Connect.h"
#include "esp_define.h"
#include "Send_data.h"
#include "Send_data_zumo.h"











void setup() { 
  connect_WIFI();
  connect_MQTT();
  client.setCallback(callback);
  
  Serial.begin(9600);
  Serial2.begin(115200);
 
  
}

void loop() {
  currentmillis = millis();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  sendData(); 
  
}
 
