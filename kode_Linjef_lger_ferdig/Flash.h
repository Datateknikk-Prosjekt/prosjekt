#ifndef Flash_H
#define Flash_H

#include <Wire.h>
#include <Zumo32U4.h>

const uint16_t maxSpeed = 200; //maksfarten til motorene. Kan synkes vist bilen skal gå saktere


Zumo32U4Buzzer buzzer; //aktiverer buzzeren 
Zumo32U4LineSensors lineSensors; //aktiverer linjesensorene
Zumo32U4Motors motors; //aktiverer motorene
Zumo32U4ButtonA buttonA; //aktiverer knapp A
Zumo32U4LCD display; //aktiverer skjermen
Zumo32U4Encoders encoders;
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
int count = 0;


void noLine(){

    beforeMillis = millis();
    while (millis()-beforeMillis <= 10000) { //har forandret seg mer en 500 på 25ms.
      int16_t nolinevalue = lineSensors.readLine(lineSensorValues); //når linjesensoren mister linja får denne en verdi på 4000.
      if (nolinevalue != 4000)break; //vist linjesensoren får en ny verdi bryter den while løkka.
      else if (millis()-beforeMillis > 2000 && millis()-beforeMillis < 2750) {
        for (int i = 0; i<210; i++){
        motors.setSpeeds(i, -i);
        count = 1;
     }
      } 
      else if (millis()-beforeMillis > 5000){ //
      motors.setSpeeds(200,100);
     }
     else {
      motors.setSpeeds(100, 100);
      int16_t nolineReturn = lineSensors.readLine(lineSensorValues); 
      if (nolineReturn != 4000)break;
     }
  }
  }



/*

void turn(){
  for (int u=0; u<90; u++){
    if (u>30){
    motors.setSpeeds(200, 200); //motorfarten blir satt til 
  }
    else{
    motors.setSpeeds(200, -200); //motorfarten blir satt til 

  }
}
}
*/

bool aboveLine(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
}



uint16_t readSensors()
{
  return lineSensors.readLine(lineSensorValues);
}



bool aboveLineDark(uint8_t sensorIndex) //funksjon som blir returnert true vist sensoren ser mye mørkt.
{
  return lineSensorValues[sensorIndex] > sensorThresholdDark;
}


bool aboveDarkSpot()
{
  return aboveLineDark(0) && aboveLineDark(1) && aboveLineDark(2) && aboveLineDark(3) && aboveLineDark(4);
}



void followline(){
  int16_t position = lineSensors.readLine(lineSensorValues); //posisjon blir definert som avlest verdi fra linjensorene
  int16_t error = position - 2000; //posisjon går fra 0-4000, error blir definert som posisjon minus 2000
  int16_t speedDifference = (error * Kp) + (Td * (error-lastError));//finner ønsket fartsforskjell på hjulene for å holde posisjon
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
  
  static const unsigned long refreshintervall = 500; //intervallet for fornyelse av variablen lastposition
  static unsigned long previousmillis = 0;
  if (millis() - previousmillis >= refreshintervall){ //if seting som blir kjørt vert 500. ms som fornyer lastposition.
    previousmillis += refreshintervall;
    lastposition = position;
  }
  if(abs(position - lastposition) > 500)
  {
    noLine();
  }
   else if(aboveDarkSpot())
    {
      motors.setSpeeds(0, 0);
      display.clear();
      display.print("Penis");
      buttonA.waitForButton();
      }
    
    
  if(aboveLine(0) && aboveLine(1) && !aboveLine(4))
    {
    motors.setSpeeds(200,200);
    display.clear();
    display.print("her");   
    }
    
  if(!aboveLine(0) && aboveLine(3) && aboveLine(4))
    {
    unsigned long NoLineMillis = millis();
    while(count == 1){
      motors.setSpeeds(200,-30);
      if (millis()-NoLineMillis == 500){
        count = 0;
        break;
          }
        }
     }
     /*
    if (aboveLine(0) && aboveLine(1) && aboveLine(2) && aboveLine(3) && aboveLine(4))
    {
      motors.setSpeeds(0,0);
      display.clear();
      display.print("ADS");
    }
    */
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //begrenser hjulfarten til å holde seg under definert maksfart
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
  
  lastError = error;
  motors.setSpeeds(leftSpeed, rightSpeed); //setter kalkulert hastighet på hvert belte
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





  




#endif
