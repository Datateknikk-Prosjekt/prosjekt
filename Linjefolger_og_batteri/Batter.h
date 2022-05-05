#ifndef Batteri_H
#define Batteri_H

#include "Linjefolger.h"



//Globale variabler batteri:
float kapasitet = 1200; //kapasitetens verdi
float kvalitet_read = EEPROM.read(0);
int currentmillis = 0;
int secundmillis = 0;
int buzz_light = 1;




void Charge(){ //lade funksjon
  if (buttonA.isPressed() && balance >= 25) { //Sjekker om bilen har fått besje om at den skal lade så mye og om den har nok penger 
    kvalitet_read = kvalitet_read - 2; //Oppdaterer kvaliteten på batteriet
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = kapasitet + 400; //setter opp hvor mye strøm som er i batteritet 
    if (kapasitet > (12*kvalitet_read)) { //passer på at det ikke blir mer strøm enn det som er mulig 
      kapasitet = 12*kvalitet_read;
    }
    zumodrift = 8; //Setter bilen inn i en case hvor den drar hjem
    Chargetime = 1; //får bilen ut av while loppen som får den til å vente på lading
    Serial1.println(kapasitet + 100); //sender den nye kapasiteten til esp-en
    delay(5000); //tar tid å lade
    balance= balance + 1400 - 25; //oppdater hvor mye penger som er igjen og gjør den klar for sending 
  }
  else if (buttonB.isPressed() && balance >= 50) {  //Sjekker om bilen har fått besje om at den skal lade så mye og om den har nok penger 
    kvalitet_read = kvalitet_read - 3;
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = kapasitet + 800;
    if (kapasitet > (12*kvalitet_read)) {
      kapasitet = 12*kvalitet_read;
    }
    zumodrift = 8; //Setter bilen inn i en case hvor den drar hjem
    Chargetime = 1; //får bilen ut av while loppen som får den til å vente på lading
    Serial1.println(kapasitet + 100); //sender den nye kapasiteten til esp-en
    delay(5000); //tar tid å lade
    balance= balance + 1400 - 50; //oppdater hvor mye penger som er igjen og gjør den klar for sending 
  }
  else if (buttonC.isPressed() && balance >= 75) { //Sjekker om bilen har fått besje om at den skal lade så mye og om den har nok penger 
    kvalitet_read = kvalitet_read - 5;
    EEPROM.update(0, kvalitet_read); //oppdaterer verdien på kavliteten i minnet
    kapasitet = 12 * kvalitet_read;
    zumodrift = 8; //Setter bilen inn i en case hvor den drar hjem
    Chargetime = 1; //får bilen ut av while loppen som får den til å vente på lading
    Serial1.println(kapasitet + 100); //sender den nye kapasiteten til esp-en
    delay(5000); //tar tid å lade
    balance= balance + 1400 - 75; //oppdater hvor mye penger som er igjen og gjør den klar for sending 
  }
}


void low_power() { //lyd funksjon som kan bli brukt for å lage lyd om bilen har lite strøm
  buzzer.playFrequency(440, 200, 15); //frekvens 440, tid 200 millis, volume 15

}


void very_low_power() { //mer ekstrem verson av low power som lager lyd og bytter mellom noen lys

  switch (buzz_light) { //switch case for å forandre på hvilke lys som lyser
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


#endif
