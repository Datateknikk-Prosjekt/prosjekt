#ifndef Send_data_H
#define Send_data_H

#include "Connect.h"
#include "esp_define.h"

//funksjon for utregning av fart
void Funkgjennomsnittsfart () {                   //funksjon for utregning av gjennomsnittsfart og maxfart 
  gjennomsnittsfart = gjennomsnittsfart + dataS;  //Legger sammen alle verdiene fra fartsmåleren
  gjennomsnitthelp++;                             //sumerer opp antall ganger farten har blitt lagt til.
  if (dataS > maxH) {                             //kjekker om farten lest er større en den største målt
    maxH = dataS; //setter den nye dataen til den største
    
    
  }
  if ((millis() - secundmillis2) > 60000) {     //kjekker om det har gått 1 min
    gjennomsnittsfart = gjennomsnittsfart/gjennomsnitthelp; //regner ut gjennomsnittsfarten 
    AvgS = gjennomsnittsfart;                     //Setter variabelen som blir sendt til nod red lik gjennomsnittsfarten
    secundmillis2 = millis();                 //reseter millis sånn at timeren fungerer
    maxS = maxH;                               //Setter variabelen som blir sendt til nod red lik makshastigheten
    gjennomsnitthelp = 0;                         //reseter variablene
    maxH = 0;
    gjennomsnittsfart = 0;
  }
}



//funksjon for utregnijng av batteri
void Powerdrain(){
  //if (dataS < 0) { //passer på at det blir gitt en positiv verdi
    //dataS = 0 - speedV;
  //}
  Hastig_V++; //holder telling på hvor mange ganger en fart har blitt lagt til
  Hastighet = Hastighet + dataS + 1; //legger til hastigheten 
  if (currentmillis - secundmillis > 1000) { //skjekker om det har gått 1 sekund
    Hastighet = Hastighet/Hastig_V; //regner ut gjennomsnitts hastigheten på det sekundet 
    Poweruse = Hastighet/2; //omregner det til energibruk
    kapasitet = kapasitet - Poweruse;//trekker brukt energi fra orginal energi
    if ((kapasitet < 300) && (Bstatus == 0)) {
      Bstatus = 1;
      Serial2.println(kapasitet);
    }
    batS = kapasitet;
    secundmillis = currentmillis; //fikser secundmillis
    Hastighet = 0; //reseter hastighets variabelen
    Hastig_V = 0; //reseter variabel
  }
}







void sendData(){
  /*if (WiFi.status() != WL_CONNECTED){ //hvis man mister tilkobling til nett
    connect_WIFI();
  }
  
  connect_MQTT(); //connecter til mqtt
  Serial.setTimeout(2000); */
  
  float g = random(20,50);
  


  if (Serial2.available() > 0) { //ser om det blir sendt noe fra bil til esp
    dataR = Serial2.readString();
    Serial2.setTimeout(50); //bryter lesing så vi kan sende og motta fortere
    dataS = dataR.toInt(); //gjør om fra int til string
  }
    
  
  
  /*Serial.print("Speed: ");
  Serial.print(dataS);
  Serial.println(" km/t");

  Serial.print("Average Speed: ");
  Serial.print(AvgS);
  Serial.println(" km/t");

  Serial.print("Max Speed: ");
  Serial.print(maxS);
  Serial.println(" km/t");
  
  Serial.print("Battery: ");
  Serial.print(batS);
  Serial.println(" %");
*/

  Funkgjennomsnittsfart ();
  Powerdrain();
  // MQTT kan bare sende strings
   // Sender til mqtt broker med topic max speed
  if (client.publish(max_speed_topic, String(maxS).c_str())) { //sender til mqtt med topic og variabel 
    Serial.println("Max speed sendt!"); //bekrefter at det er sendt i serial.print
  }
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Max speed ble ikke sendt.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(max_speed_topic, String(maxS).c_str());
  }
  // Sender til mqtt broker med topic batteri
  if (client.publish(battery_status_topic, String(batS).c_str())) {//sender til mqtt med topic og variabel 
    Serial.println("Batteri status sendt!"); //bekrefter at det er sendt
  }
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Batteri status ble ikke sendt.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(battery_status_topic, String(batS).c_str());
  }

   // Sender til mqtt broker med topic average speed
  if (client.publish(average_speed_topic, String(AvgS).c_str())) {//sender til mqtt med topic og variabel 
    Serial.println("Average speed sendt!"); //bekrefter at det er sendt
  }
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Average speed ble ikke sendt.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(average_speed_topic, String(AvgS).c_str());
  }

  // Sender til mqtt broker med topic current speed
  if (client.publish(current_speed_topic, String(dataS).c_str())) {//sender til mqtt med topic og variabel 
    Serial.println("Current speed ble sendt!");
  }
  
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Current speed ble ikke sendt");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(current_speed_topic, String(dataS).c_str());
  }
  
  // Sender til mqtt broker med topic balance
  if (client.publish(balance_topic, String(balance).c_str())) {//sender til mqtt med topic og variabel 
    Serial.println("Current speed ble sendt!");
  }
  
  //Hvis det ikke blir sendt prøves det på nytt
  else {
    Serial.println("Current speed ble ikke sendt");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(balance_topic, String(balance).c_str());
  }
  delay(sendS);       // printer ny verdi hvert .... millisekund
}


//lytter etter data fra red-node
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: "); //sier ifra hvilken topic meldingen kommer fra og hva meldingen er
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) { //meldingen kommer i char så må gjøre den om til en string 
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  

  //hvis topicen er hus går vi inn i denne if setningen
  if (String(topic) == "Hus") {

    if (messageTemp == "2"){  //dette bestemmer hvilke hus som trenger søppeltømming. 
      caseV = caseV +1;
      balance= balance+ 50;
    }
    else if (messageTemp == "4"){
      caseV = caseV+2;
      balance= balance+ 50;
      }
    else if (messageTemp == "6"){
      caseV = caseV+4;
      balance= balance+ 50;
      }
    else if (messageTemp == "5"){
      caseV = caseV-4;
      }
    else if (messageTemp == "3"){
      caseV = caseV-2;
      }
    else if (messageTemp == "1"){
      caseV = caseV - 1;
      }      
    }
    
    Serial.print("Husene som trenger søppeltømming er: ");
    
    Serial2.println(caseV);
    
   
    }
  


#endif
