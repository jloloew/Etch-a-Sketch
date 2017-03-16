
#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_MOTORS_AVAILABLE 2

enum {
    DIR_CW,
    DIR_CCW
};
typedef int motor_dir_t;

/**
 * The maximum distance the motor can safely take from its starting position, in
 * steps.
 */
extern const float motor_max_loc[NUM_MOTORS_AVAILABLE];

struct motor_move_s {
    int should_move;
    motor_dir_t dir;
};

typedef struct {
    /// A unique identifier.
    unsigned int id;
    
    unsigned int pin_dir, pin_step;
    
    struct motor_move_s next_move;
} motor_t;

typedef struct {
    float x, y;
} motor_point_t;

// Stuff for pretty printing GPIO labels.
static const int numPins = 40;
static const int numLabels = numPins + 1;
static const size_t labelWidth = 36;

typedef struct {
    // Indices correspond to physical GPIO pin numbers. The 0th element is
    // all spaces.
    char labels[numLabels][labelWidth+1];
} gpio_pinout_labels_t;

void motor_initialize(void);

int motor_init(motor_t *motor);

void motor_prepare_move(motor_t *motor, motor_dir_t dir);

/// This call blocks.
void motor_execute_move(motor_t motors[], unsigned int n_motors);

void print_gpio_labels(void);


#ifdef __cplusplus
}
#endif

#endif // MOTOR_H

