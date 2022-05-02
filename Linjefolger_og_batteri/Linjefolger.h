#ifndef LB_H
#define LB_H

#include <Wire.h>
#include <Zumo32U4.h>
#include <EEPROM.h>
#include <L3G.h>

int zumodrift;

int refreshintervall2 = 1000;
unsigned long previousmillis2;

//Globale variabler linjefølger:
const uint16_t maxSpeed = 220; //maksfarten til motorene. Kan synkes vist bilen skal gå saktere
#define NUM_SENSORS 5 
unsigned int lineSensorValues[NUM_SENSORS]; // definerer antallet linje sensorer
int16_t lastError = 0;
float dt=0;
float speedV = 0;
float Kp = 0.5 ; //5 funker for 90 grader
float Td = 2;
int16_t lastposition = 0;
const uint16_t sensorThresholdDark = 800; //grensen for om "AboveLineDark" blir returnert true.
unsigned long beforeMillis = 0;
const uint16_t sensorThreshold = 200;
int count = 1; //variabel for å unngå blindvei
int poi = 0;
int noLineCase = 1;
long darkspotmillis = 0;
//Variabler for stopping ved hus
String SHouse;
int IHouse;

int stopHouse1;
int stopHouse2;
int stopHouse3;


//Posisjonsvariabler for å assistere linjefølgeren;
int a;
int b;
int c;
int d;
int e;
int f;
int g;
int h;
int j;
int k;
int l;
int m;


//Aktiverte sumofunksjoner:
Zumo32U4Buzzer buzzer; //aktiverer buzzeren 
Zumo32U4LineSensors lineSensors; //aktiverer linjesensorene
Zumo32U4Motors motors; //aktiverer motorene
Zumo32U4LCD display; //aktiverer skjermen
Zumo32U4Encoders encoders;
Zumo32U4ButtonA buttonA; //aktiverer knapp A
Zumo32U4ButtonB buttonB; //aktiverer knapp B
Zumo32U4ButtonC buttonC; //aktiverer knapp C
L3G gyro;
Zumo32U4LCD lcd;



Void SwitchCaseHouse() {
  
  switch(zumodrift) { //Dette er en switch case der inngangsvariabelen blir styrt fra ESP32. Hver case viser til hvor på banen bilen stopper
    case 0: {
      motors.setSpeeds(0,0);
      gethouse();
      break;
      }
    case 1: { //case 1 stopper på hus 1 // followline funksjonen blir kalt på slik at bilen kjører langs linjen.
    followline();  //vist bilen motar 1 fra ESP32 vil bilen stoppe på poi 1 og kjøre videre til stasjonen
    if (poi == 2 && a == 0){ //a er her en hjelpevariabel som gjør slik at bilen kjører videre 
      motors.setSpeeds(0,0); // Samme oppbygging gjelder for alle caser.
      a++;
      delay(1500);
    }
    
     if (poi == 7) {
      motors.setSpeeds(0,0);
      zumodrift = 0;
     }
    break;
    }
    case 2: { //stopper på hus 2
      followline();
      if (poi == 3 && b == 0) {
        motors.setSpeeds(0,0);
        b++;
        delay(1500);
      }
      if (poi == 7) {
        motors.setSpeeds(0,0);
        zumodrift = 0; 
      }
      break;
    }
    case 4: { //stopper på hus 3
     followline();
      if (poi == 4 && c == 0) {
        motors.setSpeeds(0,0);
        c++;
        delay(1500);
      }
      if (poi == 7) { 
        motors.setSpeeds(0,0);
        zumodrift = 0; 
      }
      break;
      }
      case 3: { //stopper på hus 1 og 2
       followline();
     if (poi == 2 && d == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        d++;
     }
      if (poi == 3 && e == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        e++;
      }
      if (poi == 7) {
        motors.setSpeeds(0,0);
        zumodrift = 0; 
      }
        break;
      }
      case 5: { //stopper på hus 1 og 3
       followline();
     if (poi == 2 && f == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        f++;
      }
      if (poi == 4 && g == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        g++;
      }
      if (poi == 7) {
        motors.setSpeeds(0,0);
        zumodrift = 0; 
      }
        break;
      }
      case 6: { //stopper på hus 2 og 3
       followline();
     if (poi == 3 && h == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        h++;
      }
      if (poi == 4 && j == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        j++;
      }
      if (poi == 7) {
        motors.setSpeeds(0,0);
        zumodrift = 0; 
      }
        break;
      }
      
      case 7: { //stopper på hus 1, 2 og 3
       followline();
     if (poi == 2 && k == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        k++;
      }
      if (poi == 3 && l == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        l++;
      }
      if (poi == 4 && m == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        m++;
      }
      if (poi == 7) {
        motors.setSpeeds(0,0);
        zumodrift = 0; 
      }
        break;      
      }
      case 8: {
        followline();
        if (poi == 7) {
          motors.setSpeeds(0,0);
          zumodrift = 0;
        }
        break;
      }
    }
}
  
  
}



void gethouse() { //Funksjon som resetter alle posisjonsvariabler når bilen har nådd "Home"
  poi = 0;
   a = 0;
   b = 0;
   c = 0;
   d = 0;
   e = 0;
   f = 0;
   g = 0;
   h = 0;
   j = 0;
   k = 0;
   l = 0;
   m = 0;

}

//Funksjoner linjefølger:
void noLine(){ //Dette er funksjonen som blir kjørt når bilen mister linja
    beforeMillis = millis();
    while (millis()-beforeMillis <= 10000) { .
      int16_t nolinevalue = lineSensors.readLine(lineSensorValues); //når linjesensoren mister linja får denne en verdi på 4000.
      if (nolinevalue != 4000)break; //vist linjesensoren får en ny verdi bryter den while løkka.
      else if (millis()-beforeMillis > 1500 && millis()-beforeMillis < 2000) {
        for (int i = 0; i<230; i++){
        motors.setSpeeds(i, -i);
        count = 1;
        }
      } 
      else if (millis()-beforeMillis > 5000){ //Korreksjon vist den ikke finner linja
      motors.setSpeeds(200,100);
     }
     else {
      motors.setSpeeds(100, 100); //Setter ned motorfarten umiddelbart etter den mister linja
      int16_t nolineReturn = lineSensors.readLine(lineSensorValues);  
      if (nolineReturn != 4000)break;
         }
     }
  }
  

bool aboveLine(uint8_t sensorIndex) //Funksjon som blir returnert True når bilen "ser" linja
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
}



uint16_t readSensors() //
{
  return lineSensors.readLine(lineSensorValues);
}



bool aboveLineDark(uint8_t sensorIndex) //funksjon som blir returnert true vist sensoren ser mye mørkt.
{
  return lineSensorValues[sensorIndex] > sensorThresholdDark;
}


bool aboveDarkSpot() //Funksjon som blir returnert true vist alle linjesensorene ser linja
{                    // denne blir brukt til å finne poi på banen.
  motors.setSpeeds(200,200);
  return aboveLineDark(0) && aboveLineDark(1) && aboveLineDark(2) && aboveLineDark(3) && aboveLineDark(4);
}




void followline(){ //Dette er selve funkjsonen til linjefølgeren
  int16_t LFposition = lineSensors.readLine(lineSensorValues); //posisjon blir definert som avlest verdi fra linjensorene
  int16_t error = LFposition - 2000; //posisjon går fra 0-4000, error blir definert som posisjon minus 2000
  int16_t speedDifference = (error * Kp) + (Td * (error-lastError));//finner ønsket fartsforskjell på hjulene for å holde posisjon
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
  static const unsigned long refreshintervall = 500; //intervallet for fornyelse av variablen lastposition
  static unsigned long previousmillis = 0;
  if (millis() - previousmillis >= refreshintervall){ //if seting som blir kjørt vert 500. ms som fornyer lastposition.
    previousmillis += refreshintervall;
    lastposition = LFposition;
    
  }
  if(abs(LFposition - lastposition) > 250) //if setningen som ser hvist bilen mister linja
  {
    noLine();
  }
  
  else if (aboveDarkSpot()) //Setningen som ser om bilen er over poi
    {
     
    if (millis()- darkspotmillis > 2000) {
      poi++;
      for (int i = 0; i <= 25; i ++){
         motors.setSpeeds(200,200);
        }
      darkspotmillis = millis();
    }
    }
    
  if(aboveLine(0) && aboveLine(1) && !aboveLine(4)) //Hvist bilen møter et T-kryss kjører den rett frem
    {
    motors.setSpeeds(200,200);
    delay(5);
    }
    
  if(!aboveLine(0) && aboveLine(3) && aboveLine(4)) //Hvist bilen bilen har kjørt en blindvei skal den ta motsatt retning tilbake
    {
    unsigned long NoLineMillis = millis();
   
    
    while(count == 1){
      motors.setSpeeds(200,-30);
      if (aboveLine(2) && aboveLine(1) && aboveLine(3)){
        break;
      }
      if (millis()-NoLineMillis == 500){
        
        break;
          }
          
        }
      }
      
     
    
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //begrenser hjulfarten til å holde seg under definert maksfart
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
  
  lastError = error;
  motors.setSpeeds(leftSpeed, rightSpeed); //setter kalkulert hastighet på hvert belte
}







void speedometer(){ //Funksjonen som kalkulerer hastigheten
  static uint8_t lastDisplayTime;
  if ((uint8_t)(millis() - lastDisplayTime) >= 100) //Oppdateres hvert 100ms
  {
    int16_t countsLeft = encoders.getCountsAndResetLeft(); //Bruker enkoderene på hvert hjul for å kalkulere hastigheten
    int16_t countsRight = encoders.getCountsAndResetRight();
    float gCounts = (countsLeft + countsRight); //Summerer antall "counts" fra encoderne 
    dt=((gCounts)/2)/75.81; //Kalkulasjon for å gjør målingen til cm/s
    speedV = dt/(0.1);
    lastDisplayTime = millis();
    float Speedprint = constrain(speedV, 0, 30);
    Serial1.println(Speedprint); //Printer farten til Serial1/ESP32
  }
}



 void calibrateSensors() //funksjonen som kaliblerer linjesensoren
{
  delay(500); //liten delay slik at bilen ikke beveger seg presis ved påtrykk
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200); //For/if løkker som gjør at bilen roterer 180 grader i hver retning.
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}




#endif
