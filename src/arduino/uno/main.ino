#include<kettler.h>


void setup() {

  initHeadUnit();
  initDebug();
  initMotor();
  initBrakeLevel();
}

void loop() {
  updateHeadUnit();
  updateMotor();
  debugFunction();
}