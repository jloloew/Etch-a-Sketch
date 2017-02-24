
#ifndef MOTOR_H
#define MOTOR_H

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

void motor_init(void);

int motor_create(motor_t *motor);

void motor_move(const motor_t *motor, motor_dir_t dir);

#endif // MOTOR_H

