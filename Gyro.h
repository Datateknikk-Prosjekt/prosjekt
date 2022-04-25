#ifndef Gyro_H
#define Gyro_H

#include "Linjefolger.h"



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


#endif
