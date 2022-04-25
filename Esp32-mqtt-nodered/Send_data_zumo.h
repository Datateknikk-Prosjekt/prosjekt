#ifndef Send_data_zymo_H
#define Send_data_zumo_H

#include "esp_define.h"
#include "Connect.h"

  
  
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "RC") {
    
    Serial.print("Changing output to ");
    Serial2.println(messageTemp);
    if(messageTemp == "1"){
      Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("0");
      //digitalWrite(ledPin, LOW);
    }
  }
}


#endif
