#ifndef __output__
#define __output__
#include <stdint.h>

int32_t setDirectionalLed(int32_t phase);
int32_t printTicks(volatile int32_t* ticks);
int32_t printValues(int32_t angle, int32_t angVel);
int32_t initDisplay(void);

#endif