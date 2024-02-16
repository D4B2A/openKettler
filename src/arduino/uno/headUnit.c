#include<kettler.h>

static unsigned char currentHeadUnitState;

enum headUnitState {
};

void initHeadUnit() {
    //init HeadUnitState
    currentHeadUnitState = 0;

    pinMode(CLOCKWISEPIN, INPUT);
    pinMode(COUNTERCLOCKWISE, INPUT);

}

void updateHeadUnit() {
    switch currentHeadUnitState {

    }
}