
#include <stdio.h>
#include "motor.h"

int
main(int argc, char *argv[])
{
    printf("Setting up...\n");
    motor_init();
    
    // Set up a motor.
    motor_t m0;
    if (!motor_create(&m0)) {
        fprintf(stderr, "Error creating motor.\n");
        return 1;
    }
    // Print the pin numbers.
    printf("WiringPi pin numbers:\n");
    printf("Step:      %02d\n", m0.pin_step);
    printf("Direction: %02d\n", m0.pin_dir);

    printf("Beginning rotation...\n");
    while (1) {
        motor_move(&m0, DIR_CW);
    }
}

