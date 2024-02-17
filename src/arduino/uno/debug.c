#include<kettler.h>

static int currentDebugState;
static int debugFunctionStep;
static String inputString;
static bool stringComplete;
static int currentError;


static enum debugState {
  OFF,
  MANUALMOTORCONTROLL,
  AUTOMATICMOTORTEST
};


static String getDebugToken(String inputString, int TokenIndex) {
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




//initiate Debug Functions
void initDebug() {
  currentDebugStep = OFF;
  inputString = "";
  debugFunctionStep = 0;
  Serial.begin(9600);
  }



void debugFunction() {
  //Read Serial Console
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
