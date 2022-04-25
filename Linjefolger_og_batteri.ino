

#include "Linjefolger.h"
#include "Batter.h"
#include "Gyro.h"

int zumodrift = 1;


void setup()
{
  //Serial.begin(9600);
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
followline();
  /*
  constrain(kapasitet, 0, 1200);
  speedometer();
  Powerdrain();

  
  switch(zumodrift) {
    case 1: {
      //Serial.println("Case 1");
      followline();
      if (kapasitet <= 250) {
        zumodrift = 2;
      }
      break;
    }
  
  case 2: {
    Serial.println("Case 2");
    motors.setSpeeds(0, 0);
    Charge();
    if (kapasitet > 250) {
        zumodrift = 1;
      }
  }
  
  }
   */
}

  
