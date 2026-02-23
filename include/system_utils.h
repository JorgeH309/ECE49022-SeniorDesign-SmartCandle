#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include "config.h"

float ultrasonic_reading(void);
bool candle_status(void);
void align_fork(FORK_POSITION position);
void move_vertical(float distance);
void light_candle(void);
void extinguish_candle(void);

#endif