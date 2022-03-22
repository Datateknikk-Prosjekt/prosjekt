
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

void calibrateSensors() //funksjonen som kaliblerer linjesensoren
{
  delay(500); //liten delay slik at bilen ikke beveger seg presis ved påtrykk
  for(uint16_t i = 0; i < 120; i++) // Bilen leser av 120 grader i hver retning
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200); // Venstre og høyre motor får en fart på 200 og
    }                              // minus 200 rotasjoner hver
    else
    {
      motors.setSpeeds(200, -200); //bilen går tilbake til startposisjon
    }

    lineSensors.calibrate(); //linjesensoren blir kalibrert
  }
  motors.setSpeeds(0, 0); //motorfarten blir satt til 0
}

void setup() {
  lineSensors.initFiveSensors(); //linjesensorene
  display.print("Press A for Kalibrering"); //Blir vist på displayet på bilen
  buttonA.waitForButton(); 
  display.clear();
  calibrateSensors(); //kaller på kalibreringsfunksjonen når knapp A blir trykt in for første gang
  display.print("Press A for kjøring");
  buttonA.waitForButton(); // Går inn i void loop når knapp 
  display.clear();

}
void speedometer(){
    static uint8_t lastDisplayTime;
  if ((uint8_t)(millis() - lastDisplayTime) >= 100)
  {
    int16_t countsLeft = encoders.getCountsAndResetLeft();
    int16_t countsRight = encoders.getCountsAndResetRight();
    float gCounts = (countsLeft + countsRight);
    dt=((gCounts)/2)/75.81;
    speedV = dt/(0.1);
    lastDisplayTime = millis();
  display.clear();
  display.print(round(speedV)); 
  display.print("Cm/s");
  
  
  }   
}
void followline(){
  int16_t position = lineSensors.readLine(lineSensorValues); //posisjon blir definert som avlest verdi fra linjensorene
  int16_t error = position - 2000; //posisjon går fra 0-4000, error blir definert som posisjon minus 2000
  int16_t speedDifference = (error * Kp) + (Td * (error-lastError));//finner ønsket fartsforskjell på hjulene for å holde posisjon
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
  
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //begrenser hjulfarten til å holde seg under definert maksfart
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
  
  lastError = error;
  
  motors.setSpeeds(leftSpeed, rightSpeed); //setter kalkulert hastighet på hvert belte
  
}

void loop() {
  currentmillis = millis();
  followline();
  speedometer();

  
  // put your main code here, to run repeatedly:
  

}
