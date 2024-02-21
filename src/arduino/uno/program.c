#include<kettler.h>

static unsigned char currentTraining;
static unsigned int trainingTime;
static unsigned int trainingDistance;
static unsigned int trainingKcal;

static unsigned int targetTrainingTime;
static unsigned int targetTrainingDistance;
static unsigned int targetTrainingKcal;

static unsigned int lastRotationMillis;
static unsigned int currentRotationMillis;



void initProgram() {
    currentTraining = SELECTOR;
}

void quickStart() {
    currentTraining = MANUAL;
}

void resetTraining() {
    currentTraining = SELECTOR;
}

void updateProgram() {

}

unsigned char getCurrentTraining() {
    return currentTraining
}



void registerRotation() {
    lastRotationMillis = currentRotationMillis;
    currentRotationMillis = millis();
}


unsigned int getSpeed(bool unit) {
    delta = currentRotationMillis - lastRotationMillis;
    //avoid /0 
    if (delta != 0) {
        speed = 60/(delta*1000);
        if()
    }
}

void setTraining(unsigned char newTraining) {
    currentTraining = newTraining;
}


void setTragetTrainingTime(unsigned int newTargetTrainingTime) {
    targetTrainingTime = newTargetTrainingTime;
}

void setTargetTrainingDistance(unsigned int newTargetTrainingDistance) {
    targetTrainingDistance = newTargetTrainingDistance;
}

void setTragetTrainingKcal(unsigned int newTargetTrainingKcal) {
    targetTrainingKcal = newTargetTrainingKcal
}

//get Target
unsigned int getTargetTrainingTime() {
    return targetTrainingTime;
}

unsigned int getTargetTrainingDistance() {
    return targetTrainingDistance;
}

unsigned int getTargetTrainingKcal() {
    return targetTrainingKcal;
}


//get remaining
unsigned int getRemainingTrainingTime() {
    if(delta = targetTrainingTime-trainingTime>0) return delta;
    return 0;
}

unsigned int getRemainingTrainingDistance() {
    if(delta = targetTrainingDistance-trainingDistanc>0) return delta;
    return 0;
}

unsigned int getRemainingKcal() {
    if(delta = targetTrainingKcal-trainingKcal>0) return delta;
    return 0;
}