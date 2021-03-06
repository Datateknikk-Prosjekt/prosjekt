#include <Zumo32U4.h>
String Sread;
int Iread;
Zumo32U4Motors motors;
void setup() {
  Serial1.begin(115200); //starter Serial kommunikasjonen med ESP-en
}

void RC() { //funksjon for remote control
  if (Serial1.available() > 0) { //sjekker om noe har blitt motat fra espen og gjør det om til int hvis noe er motatt
    Sread = Serial1.readString();
    Serial1.setTimeout(30);
    Iread = Sread.toInt(); 
  }
  if (Iread == 8) { //gjører framover hvis den får en verdi på 8
    motors.setSpeeds(100,100);
  }
  else if (Iread == 4) { //høyre om 4 er motatt
    motors.setSpeeds(0,100);
  }
  else if (Iread == 6) { //venstre om 6 er motatt
    motors.setSpeeds(100,0);
  }
  else if (Iread == 2) { //bak om 2 er motatt
    motors.setSpeeds(-100,-100);
  }
  else if (Iread == 5) { //stopper om 5 er motatt
    motors.setSpeeds(0,0);
  }
}
