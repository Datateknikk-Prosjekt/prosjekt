#ifndef esp_define_H
#define esp_define_H

#include "PubSubClient.h" // Koble til MQTT broker
#include "WiFi.h" // Koble til WiFi esp

float gjennomsnittsfart;
int gjennomsnitthelp;
float speedV;
float maxH;
float kapasitet = 1200;

String h;


// WiFi
const char* ssid = "HUAWEI P20 Pro";                 // SSID
const char* wifi_password = "HSS12345"; // Nettverkspassord


// MQTT
const char* mqtt_server = "192.168.165.184";  // IP adresse
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
