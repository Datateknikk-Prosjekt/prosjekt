

#include "Linjefolger.h"
#include "Batter.h" //inkluderer egendefinerte biblioteker
#include "Gyro.h"

int etmillis = 0;

unsigned long SecundMillis;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200); //Starter Serial 1 for kommunikasjon til ESP32
  
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
if (kapasitet < 400 && millis() - SecundMillis > 4000) {
    SecundMillis = millis();
    very_low_power(); //Bil går i low_power_mode når kapasiteten går under 400
  }
 
  speedometer(); //Speedometer blir kalt på slik at fart blir sendt via Serial1
  if (Serial1.available()){
    SHouse = Serial1.readString(); //Zumo leser av kommando fra ESP32 
    Serial1.setTimeout(30);
    if (SHouse.toInt()>10 && SHouse.toInt() < 1300) { //Når avlest verdi er mellom desse parameter omhandler kommandoen kapasitet 
      kapasitet = SHouse.toInt() - 100;
    }
    else if (SHouse.toInt() > 1400) {
      balance = SHouse.toInt() - 1400;
    }
    else {
      zumodrift = SHouse.toInt(); // får et nummer mellom 0 og 15
    }


  }
  if (poi == 5 && kapasitet < 500) {                          //Sjekker om bilen er bed ladestasjonen og om den har lite strøm om den har det vil den vente på input om hvor mye den skal lade.
    while (Chargetime == 0) {
      motors.setSpeeds(0,0);                                  //Skrur av motorene
      Charge();                                               //kjører lade funksjoner
      very_low_power();
      Battery_reset();
    }
    Chargetime = 0;                                           //Reseter veriden som holder bilen i lade mode sånn at den kan lade igjen senere og sender hvor mye penger som er igjen til ESP-en
    Serial1.println(balance);
    poi--;
  }
  SwitchCaseHouse(); //Funksjon med switch-case på hvor på banen bilen stopper
  }



  
