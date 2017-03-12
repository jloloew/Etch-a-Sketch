
#include <stdio.h>
#include "motor.h"

int
main(int argc, char *argv[])
{
    printf("Setting up...\n");
    motor_initialize();
    
    // Set up a motor.
    motor_t m[2];
    if (motor_init(&m[0]) || motor_init(&m[1])) {
        fprintf(stderr, "Error creating motor.\n");
        return 1;
    }

    // Print the pin numbers.
    printf("WiringPi pin numbers:\n");
    printf("Step:      %02d\n", m[0].pin_step);
    printf("Direction: %02d\n", m[0].pin_dir);
    
//    print_gpio_labels();

    printf("Beginning rotation...\n");
    while (1) {
        motor_prepare_move(&m[0], DIR_CCW);
        //motor_prepare_move(&m[1], DIR_CCW);
        motor_execute_move(m, 2);
    }

    return 0;
}

