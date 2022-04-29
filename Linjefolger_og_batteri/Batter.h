#ifndef Batteri_H
#define Batteri_H

#include "Linjefolger.h"



//Globale variabler batteri:
float kapasitet = 1200; //kapasitetens verdi
float kvalitet_read = EEPROM.read(0);
float Poweruse = 0; //variabel for hvor mye som energi bilen bruker
float Hastig_V = 0; //hjelpevariabel for hastigheten
//int speedV = 0; //variabel for hva hastigeheten er
float Hastighet = 0;
int currentmillis = 0;
int secundmillis = 0;
int buzz_light = 1;
float maxH = 0;
float gjennomsnittsfart;
int gjennomsnitthelp;


//Funksjoner batteri:
void Powerdrain(){
  currentmillis = millis();
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

void Charge(){ //lade funksjon
  
  if (buttonA.isPressed()) { //skjekker om button A er presset 
    delay(15000);
    kvalitet_read = kvalitet_read - 2; //Oppdaterer kvaliteten på batteriet
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = kapasitet + 400; //setter opp hvor mye strøm som er i batteritet 
    buzzer.playFrequency(240, 200, 15); //frekvens 440, tid 200 millis, volume 15
    if (kapasitet > (12*kvalitet_read)) { //passer på at det ikke blir mer strøm enn det som er mulig 
      kapasitet = 12*kvalitet_read;
    }
  }
  else if (buttonB.isPressed()) {
    delay(20000);
    kvalitet_read = kvalitet_read - 3;
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = kapasitet + 800;
    buzzer.playFrequency(240, 200, 15); //frekvens 440, tid 200 millis, volume 15
    if (kapasitet > (12*kvalitet_read)) {
      kapasitet = 12*kvalitet_read;
    }
  }
  else if (buttonC.isPressed()) {
    delay(25000);
    kvalitet_read = kvalitet_read - 5;
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = 12 * kvalitet_read;
    buzzer.playFrequency(240, 200, 15); //frekvens 440, tid 200 millis, volume 15
  }
}



void low_power() { //under 10 varsela
  buzzer.playFrequency(440, 200, 15); //frekvens 440, tid 200 millis, volume 15

}


void very_low_power() {

  switch (buzz_light) {
    case 1: {
      buzzer.playFrequency(240, 200, 15); //frekvens 440, tid 200 millis, volume 15
      ledRed(1);
      ledYellow(0);
      ledGreen(0);

      buzz_light = 2;
      break;
      }
    case 2: {
      buzzer.playFrequency(240, 200, 15); //frekvens 440, tid 200 millis, volume 15
      ledRed(0);
      ledYellow(1);
      ledGreen(0);

      buzz_light = 3;
      break;
      }
    case 3: {
      buzzer.playFrequency(240, 200, 15); //frekvens 440, tid 200 millis, volume 15
      ledRed(0);
      ledYellow(0);
      ledGreen(1);

      buzz_light = 1;
      break;
    }
  }
}


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



#endif
