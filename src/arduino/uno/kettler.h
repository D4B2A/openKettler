#define TRIGGERTOLERANCE 20
#define TRIGGERDELAY 200
#define CYCLEDELAY 5
#define STOPTOLERANCE 10

#define LOWLIMIT 50;
#define HIGHLIMIT 500;

enum motorState {
    FORWARDS,
    FREERUNNING,
    STOP,
    BACKWARDS
};

void updateMotor() {}
void initMotor() {}
int setTargetPos(int newTargetPos) {}