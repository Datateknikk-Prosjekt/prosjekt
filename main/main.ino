#include <Wire.h>
#include <Zumo32U4.h> // inkluderer nødvendige biblitek
#include "gyro.h"
#include "batteribruk.h"


Zumo32U4ButtonA buttonA; //aktiverer knapp A
Zumo32U4ButtonB buttonB; //aktiverer knapp B
Zumo32U4ButtonC buttonC; //aktiverer knapp C



void setup()
{
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
      
}
