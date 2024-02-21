#include<kettler.h>



static char currentRotaryDecoderPosition;
static unsigned char currentHeadUnitState;

enum headUnitState {
    TRAINING,
    PROGRAM,
    AGE,
    SEX,
    DISTANCE,
    TIME,
    KCAL
};

void initHeadUnit() {
    //init HeadUnitState
    currentHeadUnitState = PROGRAM;

    pinMode(CLOCKWISEPIN, INPUT);
    pinMode(COUNTERCLOCKWISE, INPUT);

}

void updateHeadUnit() {
}

static handleRotaryEncoder(void) {
    switch(currentheadUnitState) {
        case TRAINING:


    }
}

static void writeToDisplay(String inputString){
    #if DISPLAY == "I2C-LCD"
    //code for I2C LCD here
    #endif
}