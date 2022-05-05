//add necessary libraries
#include <WiFi.h>                     

//define strings and import functions for client and server

WiFiServer server(80);
WiFiClient client;

const char* ssid = "Xperia 1_8835";
const char* password = "55147f922060";

void setup()
{

  Serial.begin(9600);         //start up Serial monitor for debugging purposes
  Serial2.begin(115200);      //start up serial 2 communication for communicating with zumo car

  WiFi.disconnect();          //resets wifi connection for the esp32
  delay(3000);                //delay of 3 seconds
  Serial.println("START");    //print "START" in the monitor so we know when the program is running
  WiFi.begin(ssid, password); //connects to the wifi
  while ((!(WiFi.status() == WL_CONNECTED))){ //prints a .. every 0.3 seconds whilst the esp32 is connecting to the wifi
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP())); //prints the IP address for the esp32
  server.begin();                   //start up the server

}


void loop()
{
  WiFiClient client = server.available();
                         
  if (!client) { 
    return; 
    }
    
  while(!client.available()){  
    delay(1); 
    }

    
    
  String ClientRequest = client.readStringUntil('\r');

  if (ClientRequest.indexOf("forward") != -1) {
    Serial2.println(8);
    Serial.println("forward");
  }
  
  else if (ClientRequest.indexOf("back") != -1)  {
    Serial2.println(2);
    Serial.println("back");
  }

  else if (ClientRequest.indexOf("left") != -1)  {
    Serial2.println(4);
    Serial.println("left");
  }
  
  else if (ClientRequest.indexOf("right") != -1)  {
    Serial2.println(6);
    Serial.println("right");
  }
  
  else {
    Serial2.println(5);
    Serial.println("stop");
  }


}
