

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
  if (poi == 5 && kapasitet < 500) { //poi 5 representerer ladestasjonen på vår bane. So vist bilen er på ladestasjon og kapasiteten er under 500
    motors.setSpeeds(0,0);           // stopper bilen
    Charge();
    Battery_reset();
  }
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



  
