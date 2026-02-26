#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

// ===============================
// Test Mode Configuration
// ===============================
typedef enum {
    LIVE,
    SUB_DEMO,
    DEBUG
} TEST_MODES;

// ===============================
// Device Selection during Debug Mode
// ===============================

typedef enum {
    MOTOR,
    ULTRASONIC,
    SERVO
} COMPONENTS;

// ===============================
// Fork Horizontal Movement Selection
// ===============================
typedef enum {
    LIGHT,
    SNUFF,
    BASE
} FORK_POSITION;

// Define Mode and Device for Testing
#define TEST_MODE SUB_DEMO
#define DEBUG_DEVICE MOTOR

// Ultrasonic Sensor
#define TRIG 28 // VALID
#define ECHO 27 // VALID

// Stepper Motors
#define STEP_ANGLE 1.8f
#define LEAD_SCREW_PITCH 0.5f // mm
#define MICROSTEPPING 1
#define XDIR 4  // INVALID, needs to be changed
#define XSTEP 3 // INVALID, needs to be changed
#define YDIR 6  // VALID
#define YSTEP 5 // VALID

// Toggle Button
#define BUTTON 21 // INVALID, needs to be changed

// IR Sensor
#define IR_PIN 7 // VALID

// Gate Driver PWM
// 150 MHz system clock
#define GATE_PWM 13 // INVALID, needs to be changed
#define CLOCK_DIVIDER 150
#define PERIOD 10000
#define DUTY_CYCLE 5000

#endif

// just use pin numbers directly on board
