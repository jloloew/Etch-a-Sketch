
#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    DIR_CW,
    DIR_CCW
};
typedef int motor_dir_t;

typedef struct {
    /// A unique identifier.
    unsigned int id;
    
    unsigned int pin_dir, pin_step;
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

void motor_move(const motor_t *motor, motor_dir_t dir);

void print_gpio_labels(void);


#ifdef __cplusplus
}
#endif

#endif // MOTOR_H

