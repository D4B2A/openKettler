#include <wiring.h>

#define TRIGGERTOLERANCE 20
#define TRIGGERDELAY 200
#define CYCLEDELAY 5
#define STOPTOLERANCE 10

#define LOWLIMIT 50
#define HIGHLIMIT 500

#define BRAKELEVEL 16

enum error {
    STATUS_OK,
    MOTORTIMEOUT
};


void updateMotor() {}
void initMotor() {}
int setTargetPos(int newTargetPos) {}

void initDebug() {}
void debugFunction() {}

int* setTimeout(int timeout) {}
bool checkTimeout(int* timeout_ptr) {}
void freeTimeout(int* timeout_ptr) {}

void initBrakeLevel() {}
void setBrakeLevel() {}
unsigned char getBrakeLevel() {}

void initHeadUnit() {}
void updateHeadUnit() {}