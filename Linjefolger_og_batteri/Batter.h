#ifndef Batteri_H
#define Batteri_H

#include "Linjefolger.h"



//Globale variabler batteri:
float kapasitet = 1200; //kapasitetens verdi
int kvalitet_read = EEPROM.read(0);
int Poweruse = 0; //variabel for hvor mye som energi bilen bruker
int Hastig_V = 0; //hjelpevariabel for hastigheten
int HastighetRead = 0; //variabel for hva hastigeheten er
int Hastighet = 0;
int currentmillis = 0;
int secundmillis = 0;




//Funksjoner batteri:
void Powerdrain(){
  if (HastighetRead < 0) { //passer på at det blir gitt en positiv verdi
    HastighetRead = 0 - HastighetRead;
  }
  Hastig_V++; //holder telling på hvor mange ganger en fart har blitt lagt til
  Hastighet = Hastighet + HastighetRead + 1; //legger til hastigheten 
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
    if (kapasitet > (12*kvalitet_read)) { //passer på at det ikke blir mer strøm enn det som er mulig 
      kapasitet = 12*kvalitet_read;
    }
  }
  else if (buttonB.isPressed()) {
    delay(20000);
    kvalitet_read = kvalitet_read - 3;
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = kapasitet + 800;
    if (kapasitet > (12*kvalitet_read)) {
      kapasitet = 12*kvalitet_read;
    }
  }
  else if (buttonC.isPressed()) {
    delay(25000);
    kvalitet_read = kvalitet_read - 5;
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = 12 * kvalitet_read;
  }
}




#endif
