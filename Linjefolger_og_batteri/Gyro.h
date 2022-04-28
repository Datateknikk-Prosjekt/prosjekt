#ifndef Gyro_H
#define Gyro_H

#include "Linjefolger.h"



String SHouse;
int IHouse;

int stopHouse1;
int stopHouse2;
int stopHouse3;

//Globale variabler gyroskop:
int gyro_r; //definerer en variabel


//Funksjon gyroskop:

void Battery_reset() { //void funksjon som leser av akselerometeret 
  gyro.read(); //leser akselereometeret 
  gyro_r = gyro.g.y;
  if (gyro_r > 800) { //oppdager at den blir snudd 
    EEPROM.update(0, 100); //oppdaterer batteriets kvaitet. Kanskje legge til en buzzer lys for å si at den er klar med en vent på button press. Kanskje sette batteri verdien tilbake til 1200.
    
    buttonA.waitForButton();
  }
}



/*
void gethouse() {
  poi = 0;
  if (Serial1.available() > 0) {
    SHouse = Serial1.readString();
    Serial1.setTimeout(30);
    IHouse = SHouse.toInt(); // får et nummer mellom 0 og 15
    if (IHouse > 3) {
      stopHouse3 = 3;
      IHouse = IHouse - 4;
      zumodrift = 3;
    }
    if (IHouse > 1) {
      stopHouse2 = 2;
      zumodrift = 2;
    }
    if (IHouse % 2 != 0 ) {
      stopHouse1 = 1;
      IHouse = IHouse - 1;
      zumodrift = 1;
    }   
}
}
*/
void gethouse() {
  poi = 0;
  a = 0;
  b = 0;
  c = 0;
  if (Serial1.available() > 0) {
    SHouse = Serial1.readString();
    Serial1.setTimeout(30);
    zumodrift = SHouse.toInt(); // får et nummer mellom 0 og 15
    /*if (IHouse == 1){
      zumodrift = 1;
    }
    if (IHouse == 2){
      zumodrift = 2;
    }
    if (IHouse == 4){
      zumodrift = 3;
    }
    if (IHouse == 3){
      zumodrift = 4;
    }
    if (IHouse == 5){
      zumodrift = 5;
    }
    if (IHouse == 6){
      zumodrift = 6;
    }*/
  }
}

#endif
