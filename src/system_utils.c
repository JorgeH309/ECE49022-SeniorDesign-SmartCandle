#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "config.h"
#include "system_utils.h"

// *********** System Utility functions *********** //

float ultrasonic_reading() {
    float distance_buffer[20] = {0};

    for (int i = 0; i < 20; i++) {
        // send 10us pulse to TRIG
        gpio_put(TRIG, 1);
        sleep_us(10);
        gpio_put(TRIG, 0);

        // wait ECHO to go high
        while (gpio_get(ECHO) == 0);

        // measure entire duration of high
        absolute_time_t start = get_absolute_time();
        while (gpio_get(ECHO) == 1);
        absolute_time_t end = get_absolute_time();

        int64_t duration = absolute_time_diff_us(start, end);

        // find distance, speed of sound = 343 m/s or 0.0343 cm/us
        // dis = (duration in us) * (speed of sound cm/us) / 2
        float distance = ((duration * 0.0343f) / 2.0f);

        printf("Distance: %.2f cm\n", distance);

        distance_buffer[i] = distance;

        sleep_ms(100);

    }

    // find average distance
    float sum = 0;
    for (int i = 0; i < 20; i++) {
        sum += distance_buffer[i];
    }

    float average_distance = sum / 20.0f;
    return average_distance;

}

bool candle_status() {
    int ir_count = 0;
    for (int i = 0; i < 20; i++) {
        if (gpio_get(IR_PIN) == 1) {
            ir_count++;
        }
        sleep_ms(10);
    }

    printf("IR count out of 20, intervals of 10 ms: %d\n", ir_count);
    return ir_count > 10;

}

void align_fork(FORK_POSITION position) {
    // move fork to position
    switch(position) {
        case LIGHT:
            // move to light position
            break;
        case SNUFF:
            // move to snuff position
            break;
        case BASE:
            // move to base position
            break;
    }
}

void move_vertical(float distance) {
    // move vertical by distance
        // set direction
    if (distance > 0) {
        gpio_put(YDIR, 1);
    }
    else {
        gpio_put(YDIR, 0);
    }

    distance = fabsf(distance);
    // step motor
    int pulses_per_rev = (360 / STEP_ANGLE) * MICROSTEPPING; // 1.8 degree with no microstepping
    int total_pulses = (int)((distance / LEAD_SCREW_PITCH) * pulses_per_rev);
    for (int i = 0; i < total_pulses; i++) {
        gpio_put(YSTEP, 1);
        sleep_us(35);
        gpio_put(YSTEP, 0);
        sleep_us(35);
    }
    sleep_ms(1000);  
}

void light_candle() {
    // read US sensor for Y
    // move motor X by constant amount
    // enable PWM for gate driver to light candle
    // move motor Y by Y
    // hold 3 seconds
    // disable PWM
    // move motor Y back
    // move motor X back
    float y_distance = ultrasonic_reading();
    align_fork(LIGHT);
    pwm_set_enabled(pwm_gpio_to_slice_num(GATE_PWM), true);
    move_vertical(y_distance);
    sleep_ms(3000);
    pwm_set_enabled(pwm_gpio_to_slice_num(GATE_PWM), false);
    move_vertical(-y_distance);
    align_fork(BASE);

}

void extinguish_candle() {
    // read US sensor for Y
    // move motor X by constant amount
    // enable PWM for gate driver to extinguish candle
    // move motor Y by Y
    // hold 3 seconds
    // disable PWM
    // move motor Y back
    // move motor X back
    float y_distance = ultrasonic_reading();
    align_fork(SNUFF);
    move_vertical(y_distance);
    sleep_ms(3000);
    move_vertical(-y_distance);
    align_fork(BASE);
    
}


// Missing:
// - button read? if we use bigger button
// - buzzer
// - init timer but dont enable until light sequence