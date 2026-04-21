#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include "config.h"

extern volatile bool check_flag;

float ultrasonic_reading(void);
bool candle_status(void);
void move_stepper_horiz(FORK fork, bool out);
void move_motor(float distance);
bool repeating_timer_callback(__unused struct repeating_timer *t);

void light_candle(void);
void extinguish_candle(void);
void move_servo(float duty_cycle);

#endif