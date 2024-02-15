#include<kettler.h>


int currentHeadUnitState;
unsigned char brakeLevel;


#define BRAKELEVEL 12


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
}

void loop() {
  updateMotor();
  debugFunction();
}


void updateBrakeLevel(unsigned char level) {
  //12 Brake Level
  //validate Level
  if(level<=BRAKELEVEL) {
    target_pos = map(level, 1, BRAKELEVEL, LOWLIMIT, HIGHLIMIT);
  }
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

void setTimeout(int timeout) {
  timeoutMillis = millis() + timeout;
}

bool checkTimeout() {
  return timeoutMillis>millis();
}
