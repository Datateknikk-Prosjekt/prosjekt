

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
  if (Serial1.available()){
    SHouse = Serial1.readString();
    Serial1.setTimeout(30);
    if (SHouse.toInt()>10) {
      kapasitet = SHouse.toInt();
    }
    else {
      zumodrift = SHouse.toInt(); // får et nummer mellom 0 og 15
    }
    
    
  }
  if (poi == 2 && kapasitet < 500) {
    motors.setSpeeds(0,0);
    Charge();
    Serial1.println(kapasitet);
  }

  switch(zumodrift) {
    case 0: {
      motors.setSpeeds(0,0);
      break;
      }
    case 1: { //case 1 stopper på hus 1
    followline(); 
    if (poi == 4 && a == 0){
      motors.setSpeeds(0,0);
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
      if (poi == 5 && b == 0) {
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
      if (poi == 6 && c == 0) {
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
     if (poi == 4 && d == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        d++;
     }
      if (poi == 5 && e == 0) {
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
     if (poi == 4 && f == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        f++;
      }
      if (poi == 6 && g == 0) {
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
     if (poi == 5 && h == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        h++;
      }
      if (poi == 6 && j == 0) {
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
     if (poi == 4 && k == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        k++;
      }
      if (poi == 5 && l == 0) {
        motors.setSpeeds(0,0);
        delay(1500);
        l++;
      }
      if (poi == 6 && m == 0) {
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
  }
}



  
