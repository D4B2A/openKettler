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
int sollWert;
int lastCycle;
bool changedSollWert;

=======
#define TRIGGERTOLERANCE 50
#define STOPTOLERANCE 20


int currentMotorState;
int sollWert;


enum motorState {
  FORWARDS,
  FREERUNNING,
  STOPP,
  BACKWARDS
  };

void setup() {
  // put your setup code here, to run once:
  currentMotorState = STOPP;
  sollWert = 200;

  changedSollWert = true;
  delayMillis = millis();
  
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
  if(delta<-TRIGGERTOLERANCE&&timeDelta>TRIGGERDELAY&&lastCYCLEDELAY) {
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
