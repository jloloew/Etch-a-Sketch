/*
 * This file just exists so the code compiles on Mac.
 * From http://wiringpi.com/reference/timing/
 */

#ifndef WIRINGPIWRAPPER_H
#define WIRINGPIWRAPPER_H

#ifdef __linux__

#include <wiringPi.h>

#else // ifndef __linux__

int wiringPiSetup(void);

void pinMode(int pin, int mode);
#define INPUT 0
#define OUTPUT 1

void digitalWrite(int pin, int value);
#define HIGH 0
#define LOW 1

void delayMicroseconds(unsigned int howLong);

#endif // __linux__

#endif // WIRINGPIWRAPPER_H

