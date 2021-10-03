#include <Adafruit_DRV8830.h>

Adafruit_DRV8830 drv;
#define DRV_I2C_ADDR 0x60

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit DRV8830 motor driver test");

  if (! drv.begin(DRV_I2C_ADDR)) {
    Serial.println("Failed to find DRV8830");
    while (1);
  }
  Serial.println("Adafruit DRV8830 found!");
}


void loop() {
  Serial.println("Forward speedup");
  drv.run(FORWARD);
  for (int i=0; i<=255; i+=5) {
    drv.setSpeed(i);
    delay(50);
  }
  Serial.println();

  Serial.println("Forward slowdown");
  for (int i=255; i>=0; i-=5) {
    drv.setSpeed(i);
    delay(50);
  }
  Serial.println();

  Serial.println("Release");
  drv.run(RELEASE);
  delay(500);

  Serial.println("Backward speedup");
  drv.run(BACKWARD);
  for (int i=0; i<=255; i+=5) {
    drv.setSpeed(i);
    delay(50);
  }
  Serial.println();

  Serial.println("Backward slowdown");
  for (int i=255; i>=0; i-=5) {
    drv.setSpeed(i);
    delay(50);
  }
  Serial.println();
}
