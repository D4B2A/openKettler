#include<kettler.h>


int currentHeadUnitState;


enum headUnitState {
  BRAKELEVEL_SIMPLE,
  BRAKELEVEL_COMPLEX,
  TIME,
  DISTANCE,
  CALORIES
};


void setup() {

  pinMode(ROT+PIN, INPUT);
  pinMode(ROT-PIN, INPUT);

  initDebug();
  initMotor();
  initBrakeLevel();
}

void loop() {
  updateMotor();
  debugFunction();
}

//BrakeLevel++
ISR() {
  //Validate Interrupt
  if(digitalRead(ROT+PIN)) {
    switch(currentHeadUnitState){
      case BRAKELEVEL_SIMPLE:
      if(brakeLevel<BRAKELEVEL){
        brakeLevel += 1;
      }
      break;
      default:
      break;
    }
  }
}

//BrakeLevel--
ISR() {
  //Validate Interrupt
  if(digitalRead(ROT-PIN)) {
    switch(currentHeadUnitState) {
      case: BRAKELEVEL_SIMPLE:
      if(brakeLevel>0) {
        brakeLevel -= 1;
      }
      break;
    }
  }
  
}
