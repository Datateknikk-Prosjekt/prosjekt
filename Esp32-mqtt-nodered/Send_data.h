#ifndef Send_data_H
#define Send_data_H

#include "Connect.h"
#include "esp_define.h"

void Funkgjennomsnittsfart () {
  gjennomsnittsfart = gjennomsnittsfart + speedV;
  gjennomsnitthelp++;
  if (speedV > maxH) {
    maxH = speedV;
    
  }
  if (currentmillis - secundmillis > 60000) {
    gjennomsnittsfart = gjennomsnittsfart/gjennomsnitthelp; //print maxh oh gjennomsnittsfarten her til espen
    
    gjennomsnitthelp = 0;
    maxH = 0;
    gjennomsnittsfart = 0;
  }
}




void Powerdrain(){
  if (speedV < 0) { //passer på at det blir gitt en positiv verdi
    speedV = 0 - speedV;
  }
  Hastig_V++; //holder telling på hvor mange ganger en fart har blitt lagt til
  Hastighet = Hastighet + speedV + 1; //legger til hastigheten 
  if (currentmillis - secundmillis > 1000) { //skjekker om det har gått 1 sekund
    Hastighet = Hastighet/Hastig_V; //regner ut gjennomsnitts hastigheten på det sekundet 
    Poweruse = 10 + 2 * Hastighet; //omregner det til energibruk
    kapasitet = kapasitet - Poweruse; //trekker brukt energi fra orginal energi
    secundmillis = currentmillis; //fikser secundmillis
    Hastighet = 0; //reseter hastighets variabelen
    Hastig_V = 0; //reseter variabel
  }
}







void sendData(){
  if (WiFi.status() != WL_CONNECTED){
    connect_WIFI();
  }
  
  connect_MQTT();
  Serial.setTimeout(2000);
  float g = random(20,50);
     
  h = Serial2.readString();
    
  
  
  Serial.print("Speed: ");
  Serial.print(h);
  Serial.println(" km/t");
  Serial.print("Battery: ");
  Serial.print(g);
  Serial.println(" %");

  // MQTT can only transmit strings
  //String hs="Hum: "+h+" % ";
  //String ts=String((float)g)+" C ";

   // Sender til mqtt broker med topic max speed
  if (client.publish(max_speed_topic, String(g).c_str())) {
    Serial.println("Batteri status sendt!"); //bekrefter at det er sendt
  }
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Batteri status ble ikke sendt.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(max_speed_topic, String(g).c_str());
  }
  // Sender til mqtt broker med topic batteri
  if (client.publish(battery_status_topic, String(g).c_str())) {
    Serial.println("Batteri status sendt!"); //bekrefter at det er sendt
  }
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Batteri status ble ikke sendt.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(battery_status_topic, String(g).c_str());
  }

   // Sender til mqtt broker med topic average speed
  if (client.publish(average_speed_topic, String(g).c_str())) {
    Serial.println("Batteri status sendt!"); //bekrefter at det er sendt
  }
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Batteri status ble ikke sendt.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(average_speed_topic, String(g).c_str());
  }

  // Sender til mqtt broker med topic current speed
  if (client.publish(current_speed_topic, String(h).c_str())) {
    Serial.println("Humidity sent!");
  }
  
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(current_speed_topic, String(h).c_str());
  }
  //client.disconnect();  // disconnect fra MQTT broker
  delay(20);       // printer ny verdi hvert .... millisekund
}





#endif
