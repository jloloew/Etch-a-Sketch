#include "wiringPiWrapper.h"
#include <stdio.h>
#ifndef __linux__
int
wiringPiSetup(void)
{
    // Empty
    return 0;
}

void
pinMode(int pin __attribute__((unused)), int mode __attribute__((unused)))
{
    // Empty
}

void
digitalWrite(int pin, int value)
{
    printf("Pin %02d -> %d\n", pin, value);
}

void
delayMicroseconds(unsigned int howLong __attribute__((unused)))
{
    // Skip the delay and do nothing
}
#endif // __linux__

