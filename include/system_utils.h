#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include "config.h"

float ultrasonic_reading(void);
bool candle_status(void);
void move_motor(float distance);
void light_candle(void);
void extinguish_candle(void);
void move_servo(float duty_cycle);
#endif