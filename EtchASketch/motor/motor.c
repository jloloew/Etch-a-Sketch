
#include <stdio.h>
#include <wiringPi.h>
#include "motor.h"

#define DELAY_US 1000

// WiringPi pin numbers
static const unsigned int dir_pins[] = {
    4,
    6
};
static const unsigned int step_pins[] = {
    2,
    12
};
#define NUM_PIN_ASSIGMENTS 2

void
motor_init(void)
{
    // Set up.
    wiringPiSetup();
}

int
motor_create(motor_t *motor)
{
    if (!motor) {
        return 1;
    }
    
    static unsigned int next_id = 0;
    unsigned int id = next_id++;
    if (id >= NUM_PIN_ASSIGMENTS) {
        return 1;
    }
    motor->pin_step = step_pins[id];
    motor->pin_dir  = dir_pins[id];
    motor->id = id;
    // Set the pins to output mode.
    pinMode(motor->pin_step, OUTPUT);
    pinMode(motor->pin_dir,  OUTPUT);
    
    return 0;
}

void
motor_move(const motor_t *motor, motor_dir_t dir)
{
    if (!motor) { // Safety first
        return;
    }
    
    // Set the direction pin
    switch (dir) {
    case DIR_CW:
        digitalWrite(motor->pin_dir, HIGH);
        break;
    
    case DIR_CCW:
        digitalWrite(motor->pin_dir, LOW);
        break;
    
    default:
        fprintf(stderr, "Invalid direction: %d\n", dir);
        return;
    }
    
    // Pulse the step pin
    digitalWrite(motor->pin_step, HIGH);
    delayMicroseconds(DELAY_US);
    digitalWrite(motor->pin_step, LOW);
    delayMicroseconds(DELAY_US);
}

