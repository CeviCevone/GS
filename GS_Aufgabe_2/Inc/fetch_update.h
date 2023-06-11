#ifndef __fetch_update__
#define __fetch_update__

#include <stdint.h>

#define STANDING 0
#define FORWARD 1 
#define BACKWARD 2 

int32_t fetch(volatile uint32_t* time,volatile int32_t* currentState,volatile int32_t* buttons);
int32_t detectPhase(volatile int32_t* i,volatile int32_t currentState); 
int32_t deltaAngle(int32_t* i,volatile uint32_t time);
int32_t totalAngle(int32_t* i); 
int32_t resetTicks(void);
int32_t getTicks(int32_t* i);
#endif