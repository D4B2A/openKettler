#include"kettler.h"

static unsigned char brakeLevel;

void initBrakeLevel() {
    brakeLevel = 1;
}

void setBrakeLevel(unsigned char brakeLevel) {
    if(level<=BRAKELEVEL) {
        setTargetPos(map(brakeLevel, 1, BRAKELEVEL, LOWLIMIT, HIGHLIMIT));
    }
}

unsigned char getBrakeLevel() {
    return brakeLevel;
}