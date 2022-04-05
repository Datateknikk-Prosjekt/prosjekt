#include <Zumo32U4.h>
#include <L3G.h>
#include <Wire.h>

L3G gyro;
Zumo32U4LCD lcd;

int gyro_r;

void Battery_reset() {
  gyro.read();
  gyro_r = gyro.g.y;
  Serial.println(gyro_r);
  delay(200);
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();
gyro.init();
gyro.enableDefault();
gyro.writeReg(L3G::CTRL1, 0b11111111);

  // 2000 dps full scale
gyro.writeReg(L3G::CTRL4, 0b00100000);

  // High-pass filter disabled
gyro.writeReg(L3G::CTRL5, 0b00000000);
delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
Battery_reset();
}
