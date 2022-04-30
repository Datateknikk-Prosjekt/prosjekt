

#include "Linjefolger.h"
#include "Batter.h"
#include "Gyro.h"
#include "RC.h"

int etmillis = 0;

unsigned long SecundMillis;

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
if (kapasitet < 400 && millis() - SecundMillis > 4000) {      //Sjekker om kapasiteten er lav og passer på at if setningen ikke blir kjørt for ofte
    SecundMillis = millis();
    very_low_power();                                         //kjører very low power som er en funksjon som lager lyd og lys
  }
  speedometer();                                              //kjører speedometeret som blir alltid kjørt
  if (Serial1.available()){                                   //Sjekker om det er noe som har blitt sendt til Serial1 og fordeler daten mottat ut fra hvor høye verdiene er.
    SHouse = Serial1.readString();
    Serial1.setTimeout(30);
    IHouse = SHouse.toInt();
    if ((IHouse > 99) && (IHouse < 500)) {
      kapasitet = IHouse - 100;                               //trekker fra det som ble lagt til for å definere dette som dennen type data 
    }
    else if (IHouse > 500) {
      balance = IHouse - 600;
    }
    else if (IHouse == 13) {
      while (RCG == 0) {                                      //Hvis 13 ble motatt går bilen inn i remote control mode.
        RC();
      }
      RCG = 0;
      Serial1.flush();                                        //Fjerner all data lagret i Serial fra når den var i RC mode sånn at det ikke kommer mass feil data til bilen.
      }
    else {
      zumodrift = SHouse.toInt(); // får et nummer mellom 0 og 15
    }
  }
  if (poi == 5 && kapasitet < 500) {                          //Sjekker om bilen er bed ladestasjonen og om den har lite strøm om den har det vil den vente på input om hvor mye den skal lade.
    while (Chargetime == 0) {
      motors.setSpeeds(0,0);
      Charge();
      very_low_power();
      Battery_reset();
    }
    Chargetime = 0;                                           //Reseter veriden som holder bilen i lade mode sånn at den kan lade igjen senere og sender hvor mye penger som er igjen til ESP-en
    Serial1.println(balance);
    poi--;
  }
  switch(zumodrift) {                                         //Switch case som styrer og passer på at bilen vet hvor den er og stopper hvis den har motat informasjon om det
    case 0: {                                                 //case 0 venter bilen på signal om hvor den skal kjøre den vil da være i ro mens den venter
      motors.setSpeeds(0,0);
      gethouse();
      break;
      }
    case 1: { //case 1 stopper på hus 1
    followline(); 
    if (poi == 2 && a == 0){
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
      case 8: { //Case for at bilen skal bare kjøre hjem igjen
        followline();
        if (poi == 7) {
          motors.setSpeeds(0,0);
          zumodrift = 0;
        }
        break;
      }
  }
}


  
