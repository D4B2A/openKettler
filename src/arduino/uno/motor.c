#include"kettler.h"

static int currentMotorState;
static int targetPos;
static int currentPos;
static bool changedTargetPos;
static unsigned long lastMillis;
static int lastCycle;

static enum motorState {
    FORWARDS,
    FREERUNNING,
    STOP,
    BACKWARDS
};


void initMotor() {
    currentMotorState = STOPP;
    changedTargetPos = true;
    lastMillis = millis();

    //init motor
    pinMode(ENABLEPIN, OUTPUT);
    pinMode(PINA, OUTPUT);
    pinMode(PINB, OUTPUT);

    //init poti
    pinMode(POTIPIN, INPUT);

    currentPos = analogRead(POTIPIN);
}

void updateMotor() {
    currentPos = analogRead(POTIPIN);
    int delta = currentPos - targetPos;

    unsigned int timeDelta = millis() - lastMillis;

    if(changedTargetPos) {
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
        setMotorState(STOP);
        lastCycle = 0;
    }
    else {
        lastCycle += 1;
    }
}

static void setMotorState(int state) {
    if(currentMotorState!=state) {
        currentMotorState = state;
        digitalWrite(ENABLEPIN, LOW);

        switch(state) {
            case FORWARDS:
                digitalWrite(PINA, LOW);
                digitalWrite(PINB, HIGH);
                digitalWrite(ENABLEPIN, HIGH);
                break;
            case STOP:
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

int setTargetPos(int newTargetPos) {
    if(LOWLIMIT<newTargetPos<HIGHLIMIT) {
        targetPos = newTargetPos;
        changedTargetPos = true;
        return 0;
    }
    //an error occured
    return 1;
}

bool isAtTargetPos() {
    return currentPos>targetPos-TRIGGERTOLERANCE&&currentPos<targetPos+TRIGGERDELAY;
}
