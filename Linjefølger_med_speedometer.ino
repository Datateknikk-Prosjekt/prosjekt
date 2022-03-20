#include <Wire.h>
#include <Zumo32U4.h> // inkluderer nødvendige biblitek

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



void setup()
{
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
  int16_t speedDifference = (error * Kp) + (2 * (error-lastError));//finner ønsket fartsforskjell på hjulene for å holde posisjon
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;
  
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); //begrenser hjulfarten til å holde seg under definert maksfart
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);
  
  lastError = error;
  
  motors.setSpeeds(leftSpeed, rightSpeed); //setter kalkulert hastighet på hvert belte
  
}



void loop(){
  followline();
  speedometer();
}
