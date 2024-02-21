#include<kettler.h>


void setup() {
  initHeadUnit();
  initDebug();
  initMotor();
  initBrakeLevel();
  initProgram();
}

void loop() {
  updateProgram();
  updateHeadUnit();
  updateMotor();
  debugFunction();
}