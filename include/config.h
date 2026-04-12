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
    SERVO,
    HORIZ_STEPPER
} COMPONENTS;

typedef enum {
    SNUFFER,
    LIGHTER
} FORK;

// Define Mode and Device for Testing
#define TEST_MODE DEBUG 
#define DEBUG_DEVICE MOTOR

// Ultrasonic Sensor
#define TRIG 11 // VALID
#define ECHO 12 // VALID

// Stepper Motors
#define STEP_ANGLE 1.8f
#define LEAD_SCREW_PITCH 0.5f // mm
#define MICROSTEPPING 1
//#define XDIR 6  // VALID
//#define XSTEP 5 // VALID
#define YDIR 9  // VALID
#define YSTEP 8 // VALID

// Toggle Button
#define BUTTON 21 // VALID

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


// Servo Motor
#define SERVO_PIN 6 // VALID
#define SERVO_CLOCK_DIVIDER 150
#define SERVO_PERIOD 10000
#define NEUTRAL_DUTY_CYCLE 1500

// Horizontal Stepper
#define XDIR 6  // VALID
#define XSTEP 5 // VALID
// ===============================
// Fork Horizontal Movement Selection
// ===============================
#define LIGHT_DUTY_CYCLE 1093 
#define SNUFF_DUTY_CYCLE 1907

#endif

// just use pin numbers directly on board
