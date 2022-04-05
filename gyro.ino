#include <Zumo32U4.h> //inkluderer bibloteker
#include <L3G.h>
#include <Wire.h>
#include <EEPROM.h>

L3G gyro;
Zumo32U4LCD lcd;

int gyro_r; //definerer en variabel

void Battery_reset() { //void funksjon som leser av akselerometeret 
  gyro.read(); //leser akselereometeret 
  gyro_r = gyro.g.y;
  if (gyro_r > 800) { //oppdager at den blir snudd 
    delay(4000); //pause 
    EEPROM.update(0, 100); //oppdaterer batteriets kvaitet. Kanskje legge til en buzzer lys for å si at den er klar med en vent på button press. Kanskje sette batteri verdien tilbake til 1200.
    
  }
}

void setup() {
  Wire.begin();  //starter wire som lar oss komuniserer med L3G (aksererometeret)
  gyro.init();   //starter aksererometeret
  gyro.enableDefault();
  gyro.writeReg(L3G::CTRL1, 0b11111111); //skrur på et filter for å ikke få mange tilfeldige verdier fra akselerometeret
    // 2000 dps full scale
  gyro.writeReg(L3G::CTRL4, 0b00100000);

    // High-pass filter disabled
  gyro.writeReg(L3G::CTRL5, 0b00000000);
}

void loop() {
  Battery_reset(); //går inn i funksjonene for battery reset
}
