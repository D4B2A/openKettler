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

unsigned long lastMillis;
int currentMotorState;
int currentHeadUnitState;
int sollWert;
int lastCycle;
bool changedSollWert;
unsigned char brakeLevel;

#define TRIGGERTOLERANCE 50
#define STOPTOLERANCE 20


#define BRAKELEVEL 12
#define LOWLIMIT 50
#define HIGHLIMIT 500

enum motorState {
  FORWARDS,
  FREERUNNING,
  STOPP,
  BACKWARDS
  };

enum headUnitState {
  BRAKELEVEL_SIMPLE,
  BRAKELEVEL_COMPLEX,
  TIME,
  DISTANCE,
  CALORIES
};


void setup() {
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
  pinMode(ROT+PIN, INPUT);
  pinMode(ROT-PIN, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(POTIPIN));
  Serial.print(",");
  Serial.println(sollWert);
  updateMotor();
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
  //Serial.println(delta);
  
  if(delta>TRIGGERTOLERANCE) {
    setMotorState(FORWARDS);
  }
  if(delta<-TRIGGERTOLERANCE) {
    setMotorState(BACKWARDS);  
  }
  if(delta<STOPTOLERANCE&&delta>-STOPTOLERANCE) {
    //Serial.println("STOPP");
    setMotorState(STOPP);
  }
}
}


void serialEvent() {
  int dataIn = Serial.parseInt(SKIP_ALL);
  //Serial.println(dataIn);
  if(dataIn==0) {
    return;
    }
  if(0<dataIn<=1023){
    sollWert = dataIn;

    changedSollWert = true;
    
  }
  else {
    Serial.println("Sollwert außerhalb des Messbereichs");
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

void updateBrakeLevel(unsigned char level) {
  //12 Brake Level
  //validate Level
  if(level<=BRAKELEVEL) {
    sollWert = map(level, 1, BRAKELEVEL, LOWLIMIT, HIGHLIMIT);
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
