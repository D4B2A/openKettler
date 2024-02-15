#include<kettler.h>

#define ENABLEPIN 2
#define PINA 3
#define PINB 4
#define POTIPIN A0
#define ROT+PIN 5
#define ROT-PIN 6

#define TRIGGERTOLERANCE 20
#define TRIGGERDELAY 500
#define CYCLEDELAY 5

#define STOPTOLERANCE 10

#define MOTORTIMEOUT 10000

unsigned long timeoutMillis;
int currentHeadUnitState;
unsigned char brakeLevel;

int currentDebugState;
String inputString;
bool stringComplete;

int currentError;
int debugFunctionStep;


enum error {
  MOTORTIMEOUT
};

enum debugState {
  OFF,
  MANUALMOTORCONTROLL,
  AUTOMATICMOTORTEST
};

#define BRAKELEVEL 12


enum headUnitState {
  BRAKELEVEL_SIMPLE,
  BRAKELEVEL_COMPLEX,
  TIME,
  DISTANCE,
  CALORIES
};


void setup() {
  //inti debug
  currentDebugState = OFF;
  inputString = "";
  debugFunctionStep = 0;

  
  Serial.begin(9600);
  Serial.setTimeout(10);

  
  
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(ROT+PIN, INPUT);
  pinMode(ROT-PIN, INPUT);

}

void loop() {
  updateMotor();
  debugFunction();
}

void serialEvent() {
  while(Serial.available) {
    int inChar = Serial.read();
    inputString += (char)inChar;
    if(inChar == "\n"){
      stringComplete = true;
      inputString = inputString.toUpperCase();
      break;
    }
  }
  //Change Debug Mode --> Global functions
  String functionToken = getDebugToken(inputString, 0);
  switch(functionToken){
    case "EXIT":
      //Provide Feedback
      if(currentDebugState != OFF) {
        Serial.println("Exited debug programm");
      }
      else {
        Serial.println("You're not in a debug programm")
      }
      currentDebugState = OFF;
      break;
    case "MANUALMOTORCONTROLL":
      //provide feedback
      if(currentDebugState != MANUALMOTORCONTROLL) {
        Serial.println("Starting MANUALMOTORCONTROLL");
      }
      else {
        Serial.println("You're allready in MANUALMOTORCONTROLL");
      }
      currentDebugState = MANUALMOTORCONTROLL;
      break;
    case: "AUTOMATICMOTORTEST":
      if(currentDebugState!=AUTOMATICMOTORTEST){
        Serial.println("Starting AUTOMATICMOTORTEST");
      }
      else {
        Serial.println("You're allready in AUTOMATICMOTORTEST");
      }
      currentDebugState = AUTOMATICMOTORTEST;
    default:
      break;
  }


  //Execute Debug Mode Input
  switch(currentDebugState){
    case MANUALMOTORCONTROLL:
      int dataIn = toInt(getDebugToken(inputString,0));
      if(!setTargetPos(dataIn)){
        Serial.println("target position outside allowed borders");
      }
      break;
    default:
      //do nothing
      break;
  }
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

String getDebugToken(String inputString, int TokenIndex) {
  int length = inputString.length();
  int currentIndex = 0;
  bool isToken = false;
  String OutputString = "";
  for(int i = 0; i<length; i++) {
    if(inputString[i]==" ") {
      //just count one blank
      if(isToken){
        currentIndex += 1;
        if(currentIndex>TokenIndex){
          return OutputString;
        }
      }
    }
    else {
      isToken = true;
      if(currentIndex==TokenIndex){
        OutputString += inputString[i];
      }
    }
  }
}


void debugFunction() {
  switch(currentDebugState) {
    case MANUALMOTORCONTROLL:
      Serial.print("current position:");
      Serial.print(analogRead(POTIPIN));
      Serial.print(", target position:");
      Serial.println(target_pos);
      break;
    case AUTOMATICMOTORTEST:
      //Send Serial Data
      Serial.print("current position:");
      Serial.print(analogRead(POTIPIN));
      Serial.print(", target position:");
      Serial.println(target_pos);

      switch(debugFunctionStep) {
        case 0:
          Serial.println("Starting Test");
          debugFunctionStep += 1;
          setTimeout(MOTORTIMEOUT);
        case 1:
          //Move to lowest position
          setTargetPos(LOWLIMIT);
          if(checkTimeout) {
            Serial.println("Motortimeout triggered during autotest");
            error = MOTORTIMEOUT;
            currentDebugState = OFF;
          }
          if(isAtTargetPos()) {
            debugFunctionStep += 1;
            setTimeout(MOTORTIMEOUT);
          }
          break;
        case 2:
          //Move to highest position
          setTargetPos(HIGHLIMIT);
          if(checkTimeout) {
            Serial.println("Motortimeout triggered during autotest")
            errror = MOTORTIMEOUT;
            currentDebugState = OFF;
          }
          if(isAtTargetPos()) {
            debugFunctionStep += 1;
          }
      }
    default:
      //Do nothing
      break;
  }
}



void setTimeout(int timeout) {
  timeoutMillis = millis() + timeout;
}

bool checkTimeout() {
  return timeoutMillis>millis();
}
