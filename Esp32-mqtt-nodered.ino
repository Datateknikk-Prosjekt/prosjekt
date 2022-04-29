

//lokale bibliotek
#include "Connect.h"
#include "esp_define.h" //alle variabler og definisjoner
#include "Send_data.h" //sending av data zumo - esp - nodered



void setup() { 
  Serial.begin(9600);
  
  connect_WIFI(); //kobler til nettet
  delay(50);
  connect_MQTT(); //kobler til mqtt
  client.setCallback(callback); //gjør at den vil høre etter innkommende data
  
  Serial2.begin(115200); //starter serial2 for kommunikasjon mellom zumo og esp

  
}

void loop() {

  currentmillis = millis();

  if (WiFi.status() != WL_CONNECTED){ //hvis vi blir disconnected fra wifi
    connect_WIFI();
  }

  if (!client.connected()) { //hvis vi blir disconnected fra mqtt
    reconnect();
  }
  client.loop(); //gjør at søket etter innkommende signal kjører hele tiden

  
  sendData();  //sender data til node red og zumo
  
  
  
  
}
 
