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
#define DEBUG_DEVICE ULTRASONIC

// Ultrasonic Sensor
#define TRIG 11 // VALID
#define ECHO 12 // VALID

// Stepper Motors
#define STEP_ANGLE 1.8f
#define LEAD_SCREW_PITCH 0.5f // mm
#define MICROSTEPPING 1
#define XDIR 6  // INVALID, needs to be changed
#define XSTEP 5 // INVALID, needs to be changed
#define YDIR 9  // VALID
#define YSTEP 8 // VALID

// Toggle Button
#define BUTTON 21 // INVALID, needs to be changed

// IR Sensor
#define IR_PIN 7 // VALID

// Gate Driver PWM
// 150 MHz system clock
#define GATE_PWM 18 // VALID
#define CLOCK_DIVIDER 150
#define PERIOD 10000
#define DUTY_CYCLE 5000


#define SPEAKER_PWM 17 // VALID
#define SPEAKER_CLOCK_DIVIDER 1500
#define SPEAKER_PERIOD 200
#define SPEAKER_DUTY_CYCLE 100
#endif

// just use pin numbers directly on board
