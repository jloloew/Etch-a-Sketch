
#include <stdio.h>
#include <stdlib.h>
#include "motor.h"

static void __attribute__((noreturn))
usage(const char *argv[])
{
    printf("Usage: %s [num-steps]\n", argv[0]);
    exit(1);
}

int
main(int argc, const char *argv[])
{
    if (argc > 2) {
        usage(argv);
    }
    
    motor_initialize();
    
    // Set up a motor.
    motor_t m[2];
    if (motor_init(&m[0]) || motor_init(&m[1])) {
        fprintf(stderr, "Error creating motor.\n");
        return 1;
    }

    // Get the number of steps to perform.
    unsigned long num_steps = 0;
    if (argc > 1) {
        num_steps = strtoul(argv[1], NULL, 10);
        if (num_steps == 0) {
            usage(argv);
        }
    }

    // Print the pin numbers.
    printf("WiringPi pin numbers:\n");
	printf("Motor 0:\n");
    printf("    Step:      %02d\n", m[0].pin_step);
	printf("    Direction: %02d\n", m[0].pin_dir);
	printf("Motor 1:\n");
	printf("    Step:      %02d\n", m[1].pin_step);
	printf("    Direction: %02d\n", m[1].pin_dir);
	
//    print_gpio_labels();

    if (num_steps > 0) {
        printf("Will move %lu steps\n", num_steps);
    }
    
    printf("Beginning rotation...\n");
    
    int keep_going = 1;
    while (keep_going) {
        // If the user specified num_steps, stop eventually. Otherwise, loop
        // forever.
        if (num_steps > 0) {
            keep_going = --num_steps > 0;
        }
        
#ifdef MOTOR_MOVE_LEFT
        motor_prepare_move(&m[0], DIR_CCW);
#elif defined MOTOR_MOVE_RIGHT
		motor_prepare_move(&m[0], DIR_CW);
#endif
		
#ifdef MOTOR_MOVE_DOWN
		motor_prepare_move(&m[1], DIR_CCW);
#elif defined MOTOR_MOVE_UP
		motor_prepare_move(&m[1], DIR_CW);
#endif
        motor_execute_move(m, 2);
    }

    return 0;
}

