#ifndef Gyro_H
#define Gyro_H

#include "Linjefolger.h"




//Globale variabler gyroskop:
int gyro_r; //definerer en variabel


//Funksjon gyroskop:

void Battery_reset() { //void funksjon som leser av akselerometeret 
  gyro.read(); //leser akselereometeret 
  gyro_r = gyro.g.y; 
  if (gyro_r > 800 && balance >= 200) { //Sjekker om bilen blir snudd og om bilen har nok penger til å bytte batteri 
    EEPROM.update(0, 100); //oppdaterer batteriets kvaitet. Kanskje legge til en buzzer lys for å si at den er klar med en vent på button press. Kanskje sette batteri verdien tilbake til 1200.
    low_power(); //lager lyd funksjon
    zumodrift = 8; //Setter bilen i fra hjem mode
    kapasitet = 1200; //Setter batteriet til maks
    balance = balance + 1200; //Trekker fra hvor mye penger som er brukt og gjør det klar for sending ved å legge til 1400 sånn at esp-en skjønner at dette er en penge verdi
    delay(2000); //venter litt
    Serial1.println(kapasitet + 100); //sender den nye veriden på hvor mye strøm som er igjen på bilen.
    Chargetime = 1;   //bryter while loopen
    buttonA.waitForButton();  //venter på at butten A skal bli presset sånn at bilen ikke gjører før brukeren er klar
  }
}



#endif
