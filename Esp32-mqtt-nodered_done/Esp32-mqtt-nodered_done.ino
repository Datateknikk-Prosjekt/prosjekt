

//lokale bibliotek
#include "Connect.h"
#include "esp_define.h" //alle variabler og definisjoner
#include "Send_data.h" //sending av data zumo - esp - nodered



void setup() { 
  Serial.begin(9600);
  Serial2.begin(115200); //starter serial2 for kommunikasjon mellom zumo og esp

  connect_WIFI(); //kobler til nettet
  delay(50);
  connect_MQTT(); //kobler til mqtt
  client.setCallback(callback); //gjør at den vil høre etter innkommende data
  
  
  
}

void loop() {

  currentmillis = millis(); //oppdaterer currentmillis

  if (WiFi.status() != WL_CONNECTED){ //hvis vi blir disconnected fra wifi så 
    connect_WIFI();                   //prøver den å koble til igjen
    
  }

  if (!client.connected()) { //hvis vi blir disconnected fra mqtt
    reconnect();             //prøver den å koble til igjen
  }
  client.loop(); //gjør at søket etter innkommende signal kjører hele tiden
  
  sendData();  //sender og mottar data til node red og zumo
}
 
