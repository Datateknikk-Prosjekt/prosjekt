//definerer bibliotek
#ifndef Connect_H
#define Connect_H




//lokale biblioteker
#include "esp_define.h"

//WiFiServer server(80);

void reconnect() {
  // Loop helt til vi er koblet på mqtt igjen
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Subscribe til topic 
      client.subscribe("Hus");
      client.subscribe("RC");
    } else {
      Serial.print("failed");
      // Venter 5 sek før den prøver igjen
      delay(5000);
    }
  }
}


//Funskjon for mqtt
void connect_MQTT(){
  client.setServer(mqtt_server, 1883); //kobler til mqtt med server og port


  // Kobler til mqtt med brukernavn og passord. Printer i serial hvis den klarer det eller ikke.
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
    client.subscribe("Hus"); //subscriber på topic
    client.subscribe("RC");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}
//Funksjon for å kobler Esp til wifi ved hjelp av ssid og passord. Printer en "." mens den prøver å koble til. 
void connect_WIFI(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);

 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); //venter på å koble til
  }
//wifi er koblet til
  Serial.println("WiFi connected");
  Serial.println("Your IP is:");
  Serial.println((WiFi.localIP()));
  
}

#endif
