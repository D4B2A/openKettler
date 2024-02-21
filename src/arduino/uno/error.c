#include<kettler.h>

#define MOTORTIMEOUT 100000


int* setTimeout(int timeout) {
    int* prt = malloc(sizeof(int));
    (*prt) = millis()+timeout;
    return prt;
}

bool checkTimeout(int* timeout_ptr) {
    return (*timeout_ptr)>millis();
}

void freeTimeout(int* timeout_ptr) {
    free(timeout_ptr);
}