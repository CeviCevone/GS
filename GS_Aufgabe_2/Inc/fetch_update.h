#ifndef __fetch_update__
#define __fetch_update__

#define STANDING 0
#define FORWARD 1 
#define BACKWARD 2 

int fetch(unsigned int* time, int* currentState, int* buttons);
int detectPhase(int* i,int currentState); 
int deltaAngle(int* i);
int totalAngle(int* i); 
int resetTicks(void);
int getTicks(int* i);
#endif