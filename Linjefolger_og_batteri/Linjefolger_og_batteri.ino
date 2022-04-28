

#include "Linjefolger.h"
#include "Batter.h"
#include "Gyro.h"

int etmillis = 0;


void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200);
  
  //setup linjefølger:
  lineSensors.initFiveSensors(); //linjesensorene
  display.print("Press A for Kalibrering"); //Blir vist på displayet på bilen
  buttonA.waitForButton(); 
  display.clear();
  calibrateSensors(); //kaller på kalibreringsfunksjonen når knapp A blir trykt in for første gang
  display.print("Press A for kjøring");
  buttonA.waitForButton();
  
  //Setup batteri:
  Wire.begin();  //starter wire som lar oss komuniserer med L3G (aksererometeret)
  gyro.init();   //starter aksererometeret
  gyro.enableDefault();
  gyro.writeReg(L3G::CTRL1, 0b11111111); //skrur på et filter for å ikke få mange tilfeldige verdier fra akselerometeret
    // 2000 dps full scale
  gyro.writeReg(L3G::CTRL4, 0b00100000);
    // High-pass filter disabled
  gyro.writeReg(L3G::CTRL5, 0b00000000);


}


void loop(){
  speedometer();
 // if (millis() - etmillis > 500) {
   // Serial1.println(zumodrift);
    //etmillis = millis();
  //}
  switch(zumodrift) {
    case 0: {
      gethouse();
      break;
      }
    case 1: {
    followline();
    if (poi == 1){
      motors.setSpeeds(0,0);
      delay(1500);
    }
      if (stopHouse2 == 2){
        zumodrift = 2;
    }
      else if (stopHouse3 == 3){
        zumodrift = 3;
      }
      else {
        zumodrift == 4;
      }
    break;
    }
    case 2: {
      followline();
      if (poi == 2){
        motors.setSpeeds(0,0);
        delay(1500);
        if (stopHouse3 == 3){
          zumodrift = 3;
        }
        else {
          zumodrift = 0;
        }
      }
      break;
    }
    case 3: {
      followline();
      if (poi == 3){
        motors.setSpeeds(0,0);
        delay(1500);
        zumodrift = 4;
        }
      break;
      }
      case 4: {
        followline();
        if (poi == 4) {
          poi = 0;
          zumodrift = 0;
          delay(1500);
        }
        break;
      }
  }

}

  
