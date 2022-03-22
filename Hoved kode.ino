
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA; //definerer buttons
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4ButtonD buttonD;

float kapasitet = 1200; //kapasitetens verdi
float kvalitetsverdien = 1; //verdi på standen av batteriet
int Poweruse = 0; //variabel for hvor mye som energi bilen bruker
int Hastig_V = 0; //hjelpevariabel for hastigheten
int HastighetRead = 0; //variabel for hva hastigeheten er
int Hastighet = 0;
int currentmillis = 0;
int secundmillis = 0;


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
  
  return;
}

void Charge(){ //lade funksjon
  
  if (buttonA.isPressed()) { //skjekker om button A er presset 
    delay(15000);
    kvalitetsverdien = kvalitetsverdien - 0.015 //Oppdaterer kvaliteten på batteriet
    kapasitet = kapasitet + 400 //setter opp hvor mye strøm som er i batteritet 
    if (kapasitet > (1200*kvalitetsverdien)) { //passer på at det ikke blir mer strøm enn det som er mulig 
      kapasitet = 1200*kvalitetsverdien;
    }
  }
  if (buttonB.isPressed()) {
    delay(20000);
    kvalitetsverdien = kvalitetsverdien - 0.025
    kapasitet = kapasitet + 800
    if (kapasitet > (1200*kvalitetsverdien)) {
      kapasitet = 1200*kvalitetsverdien;
    }
  }
  if (buttonC.isPressed()) {
    delay(25000);
    kvalitetsverdien = kvalitetsverdien - 0.04
    kapasitet = 1200 * kvalitetsverdien 
  }
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  currentmillis = millis();

  
  // put your main code here, to run repeatedly:
  

}
