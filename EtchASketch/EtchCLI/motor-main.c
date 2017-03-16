
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "motor.h"

static unsigned long num_steps_taken = 0;

/// Print the total number of steps taken, then exits.
static void __attribute__((noreturn))
print_steps_taken_and_exit(void)
{
    printf("\n\nTotal number of steps taken: %lu\n", num_steps_taken);
    _exit(0);
}

/// Signal handler. Prints the total number of steps taken, then exits.
static void __attribute__((noreturn))
int_handler(int sig __attribute__((unused)))
{
    print_steps_taken_and_exit();
}

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

    // Set up the motors.
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
    const unsigned long target_num_steps = num_steps;

    // Print the pin numbers.
    printf("WiringPi pin numbers:\n");
	printf("Motor 0:\n");
    printf("    Step:      %02d\n", m[0].pin_step);
	printf("    Direction: %02d\n", m[0].pin_dir);
	printf("Motor 1:\n");
	printf("    Step:      %02d\n", m[1].pin_step);
	printf("    Direction: %02d\n", m[1].pin_dir);
    printf("\n");

//    print_gpio_labels();

    if (target_num_steps > 0) {
        printf("Will move %lu steps.\n", num_steps);
    }

    // Install signal handler to print the total number of steps taken at exit.
    struct sigaction act;
    act.sa_handler = int_handler;
    sigaction(SIGINT, &act, NULL);
    atexit(print_steps_taken_and_exit);

    printf("Beginning rotation...\n");
    if (target_num_steps > 0) {
        printf("%05lu / %05lu", 0LU, target_num_steps);
        fflush(stdout);
    }

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

        // Update the current step count.
        if (++num_steps_taken % 10 == 0) {
            printf("\r%05lu", num_steps_taken);
            fflush(stdout);
        }
    }

    return 0;
}
