#ifndef LB_H
#define LB_H

#include <Wire.h>
#include <Zumo32U4.h>
#include <EEPROM.h>
#include <L3G.h>



//Globale variabler linjefølger:
const uint16_t maxSpeed = 300; //maksfarten til motorene. Kan synkes vist bilen skal gå saktere
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
int count = 1;




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



//Funksjoner linjefølger:
void noLine(){  //funksjonen som blir kalt på når sensoren mister linja
    beforeMillis = millis(); //Variabelen får en ny tidsverdi
    while (millis()-beforeMillis <= 10000) { 
      int16_t nolinevalue = lineSensors.readLine(lineSensorValues); //når linjesensoren mister linja får denne en verdi på 4000.
      if (nolinevalue != 4000)break; //vist linjesensoren får en ny verdi (finner linja) bryter den while løkka.
      else if (millis()-beforeMillis > 1500 && millis()-beforeMillis < 2250) { //etter 1500ms uten linje snur bilen
        for (int i = 0; i<230; i++){ //for løkke som snur bilen
        motors.setSpeeds(i, -i);
        count = 1;
     }
      } 
      else if (millis()-beforeMillis > 5000){ //etter 5 sekunder tar bilen en høyresving hvis den ikke finner linja for
      motors.setSpeeds(200,100);              // erfaringsmessi er det ikke alltid bilen leser av en verdi på denne sida
     }
     else {
      motors.setSpeeds(100, 100); //etter bilen har snudd 180 grader kjører den tilbake og finner linja den forlot
      int16_t nolineReturn = lineSensors.readLine(lineSensorValues);  
      if (nolineReturn != 4000)break; //når den finner tilbake til linja får variabelen en ny verdi og løkka blir brutt
     }
  }
  }




bool aboveLine(uint8_t sensorIndex) //funksjon som blir returnert "True" når linjesensoren ser en linje.
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
}



uint16_t readSensors() //tar verdiene avlest fra linjesensorene og returnerer en estimert plassering
{
  return lineSensors.readLine(lineSensorValues);
}



bool aboveLineDark(uint8_t sensorIndex) //funksjon som blir returnert "True" hvis sensoren ser mer mørkt en "SensorThresholdDark"
{
  return lineSensorValues[sensorIndex] > sensorThresholdDark;
}


bool aboveDarkSpot() //Blir returnert "True" hvis hver sensor ser linja
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
  if(abs(position - lastposition) > 500) //hvis posisjonsendingen overstiger en verdi på 500 på 500ms blir denne funksjonen aktivert
  {
    noLine();
  }
   else if(aboveDarkSpot()) //Blir aktivert når "aboveDarkSpot" blir returnert "True"
    {
      motors.setSpeeds(0, 0); //foreløpig setter den bare motorfarten til 0, men sette inn en variabel her som styrer en switch case??
      buttonA.waitForButton();
      }
    
    
  if(aboveLine(0) && aboveLine(1) && !aboveLine(4))
    {
    motors.setSpeeds(200,200); //Bilen kjører rett frem i et T-kryss
    delay(5); // delay må påføres for at bilen skal kjøre rett i krysse hver gang
    }
    
  if(!aboveLine(0) && aboveLine(3) && aboveLine(4)) //funksjon som blir aktivert når bilen ser en høyre-sving
    {
    unsigned long NoLineMillis = millis();
    while(count == 1){
      motors.setSpeeds(200,-30);
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
  //display.clear();
  //display.print(round(speedV)); 
  //display.print("Cm/s");
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
