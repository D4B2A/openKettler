#define ENABLEPIN 2
#define PINA 3
#define PINB 4
#define POTIPIN A0

#define TRIGGERTOLERANCE 20
#define TRIGGERDELAY 500
#define CYCLEDELAY 5

#define STOPTOLERANCE 10

unsigned long lastMillis;
int currentMotorState;
int currentDebugState;
String inputString;
bool stringComplete;

int sollWert;
int lastCycle;
bool changedSollWert;

#define TRIGGERTOLERANCE 50
#define STOPTOLERANCE 20

enum debugState {
  OFF,
  MANUALMOTORCONTROLL
};

enum motorState {
  FORWARDS,
  FREERUNNING,
  STOPP,
  BACKWARDS
  };

void setup() {
  currentDebugState = OFF;
  inputString = "";

  // put your setup code here, to run once:
  currentMotorState = STOPP;
  sollWert = 200;

  changedSollWert = true;
  lastMillis = millis();
  
  Serial.begin(9600);
  Serial.setTimeout(10);

  
  
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(ENABLEPIN, OUTPUT);
  pinMode(PINA, OUTPUT);
  pinMode(PINB, OUTPUT);
  pinMode(POTIPIN, INPUT);
  setMotorState(FORWARDS);

}

void loop() {
  updateMotor();
  debugFunction();
}

void updateMotor() {
  int istWert = analogRead(POTIPIN);
  int delta = istWert-sollWert;


  unsigned int timeDelta = millis() - lastMillis;
  //Serial.println(delta);

  if(changedSollWert) {
    //change values to allow for trigger
    timeDelta += TRIGGERDELAY + 1;
    lastCycle += CYCLEDELAY + 1;
  }
  
  if(delta>TRIGGERTOLERANCE&&timeDelta>TRIGGERDELAY&&lastCycle>CYCLEDELAY) {
    setMotorState(FORWARDS);
  }
  if(delta<-TRIGGERTOLERANCE&&timeDelta>TRIGGERDELAY&&lastCycle>CYCLEDELAY) {
    setMotorState(BACKWARDS);  
  }
  if(delta<STOPTOLERANCE&&delta>-STOPTOLERANCE) {
    lastMillis = millis();
    setMotorState(STOPP);
    lastCycle = 0;
  }
  else {
    lastCycle += 1;
  
    if(delta>TRIGGERTOLERANCE) {
      setMotorState(FORWARDS);
    }
    if(delta<-TRIGGERTOLERANCE) {
      setMotorState(BACKWARDS);  
    }
    if(delta<STOPTOLERANCE&&delta>-STOPTOLERANCE) {
      setMotorState(STOPP);
    }
  }
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
        Serial.println("Started MANUALMOTORCONTROLL");
      }
      else {
        Serial.println("You're allready in MANUALMOTORCONTROLL");
      }
      currentDebugState = MANUALMOTORCONTROLL;
      break;
    default:
      break;
  }


  //Execute Debug Mode Input
  switch(currentDebugState){
    case MANUALMOTORCONTROLL:
      int dataIn = toInt(getDebugToken(inputString,0));
      if(0<dataIn<=1023){
        sollWert = dataIn;
        changedSollWert = true;
      }
      else {
        Serial.println("Sollwert außerhalb des Messbereichs");
      }
      break;
    default:
      //do nothing
      break;
  }
}


void setMotorState(int state) {
  if(currentMotorState!=state) {
    currentMotorState = state;
    //Serial.println(state);
    //Switch into freerunning for change
    digitalWrite(ENABLEPIN, LOW);
    switch(state) {
      case FORWARDS:
      digitalWrite(PINA, LOW);
      digitalWrite(PINB, HIGH);
      digitalWrite(ENABLEPIN, HIGH);
      break;
      case STOPP:
      digitalWrite(PINA, LOW);
      digitalWrite(PINB, LOW);
      digitalWrite(ENABLEPIN, HIGH);
      break;
      case BACKWARDS:
      digitalWrite(PINA, HIGH);
      digitalWrite(PINB, LOW);
      digitalWrite(ENABLEPIN, HIGH);
      break;
      default:
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
      Serial.print(analogRead(POTIPIN));
      Serial.print(",");
      Serial.println(sollWert);
      break;
    default:
      //Do nothing
      break;
  }
}