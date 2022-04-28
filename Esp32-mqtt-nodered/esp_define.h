#ifndef esp_define_H
#define esp_define_H

#include "PubSubClient.h" // Koble til MQTT broker
#include "WiFi.h" // Koble til WiFi esp

//variabler for utregninger for fart og batteri
float gjennomsnittsfart;
int gjennomsnitthelp;
float speedV;
float maxH;
float kapasitet = 1200;

//variabler for utregninger for fart og batteri
unsigned long currentmillis;
unsigned long secundmillis;
float Hastig_V;
float Hastighet;
float Poweruse;



//Variabler for motta data fra zumo og sende til node red
String dataR;
int dataS;
int sendS = 20; //hvor ofte den sender til node red
float batS;
float AvgS;
float maxS;


//variabler for sending av data til zumo fra esp
int caseV;
unsigned long twoMillis;


// WiFi
const char* ssid = "AndroidAP";                 // SSID
const char* wifi_password = "hei45678"; // Nettverkspassord


// MQTT
const char* mqtt_server = "192.168.186.184";  // IP adresse
const char* mqtt_username = "MangoPie"; // MQTT username
const char* mqtt_password = "MangoPie"; // MQTT password
const char* clientID = "Zumo/Esp"; // Esp navn
//mqtt topics
const char* current_speed_topic = "Current Speed";
const char* battery_status_topic = "Battery Status";
const char* average_speed_topic = "Avg. Speed";
const char* max_speed_topic = "Max speed";



// Starter wifi og mqtt
WiFiClient espClient;
// lytter på port 1883
PubSubClient client(espClient);

#endif
