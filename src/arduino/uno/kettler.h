#include "wiring.h"

#define TRIGGERTOLERANCE 20
#define TRIGGERDELAY 200
#define CYCLEDELAY 5
#define STOPTOLERANCE 10

#define LOWLIMIT 50
#define HIGHLIMIT 500

#define BRAKELEVEL 16

enum speedUnit {
    RPM,
    KMH
};


enum error {
    STATUS_OK,
    MOTORTIMEOUT
};

enum training {
    SELECTOR,
    MANUAL,
    PROGRAM,
    PERSONAL,
    HRC
};


void updateMotor();
void initMotor();
int setTargetPos(int);

void initDebug();
void debugFunction();

int* setTimeout(int);
bool checkTimeout(int*);
void freeTimeout(int*);

void initBrakeLevel();
void setBrakeLevel();
unsigned char getBrakeLevel();

void initHeadUnit();
void updateHeadUnit();

void initProgram();
void getCurrentProgram();
void quickStart();
void resetTraining();
void updateProgram();
unsigned int getTargetTrainingTime();
unsigned int getTragetTrainingDistance();
unsigned int getTragetTrainingKcal();
unsigned int getRemainingTrainingTime();
unsigned int getRemainingTrainingDistance();
unsigned int getRemainingTrainingKcal();