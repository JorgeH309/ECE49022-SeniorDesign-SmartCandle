#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "config.h"
#include "system_utils.h"

// *********** System Utility functions *********** //
#define count 5
float ultrasonic_reading() {
    float distance_buffer[count] = {0};
    for (int i = 0; i < count; i++) {
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
    for (int i = 0; i < count; i++) {
        sum += distance_buffer[i];
    }

    float average_distance = sum / count;
    printf("Average Distance: %.2f cm\n", average_distance);
    return average_distance;

}

bool candle_status() {
    int ir_count;

    ir_count = 0;
    for (int i = 0; i < 20; i++) {
        if (gpio_get(IR_PIN) == 0) {
            ir_count++;
        }
        sleep_ms(10);
    }

    printf("IR count out of 20, intervals of 10 ms: %d\n", ir_count);
    //sleep_ms(1000);
    return ir_count > 10;

}

void move_stepper_horiz(FORK fork, bool out) {
    uint dir_pin;
    uint step_pin;

    dir_pin = YDIR;
    step_pin = YSTEP;
    // move horizontal by constant distance

    int degrees = 50;
    // set direction
    if (fork == LIGHTER) {
        if (out) {
            gpio_put(dir_pin, 1);
        }
        else {
            gpio_put(dir_pin, 0);
        }
    }
    else {
        if (out) {
            gpio_put(dir_pin, 0);
        }
        else {
            gpio_put(dir_pin, 1);
        }
    }

    // step motor
    int pulses_to_move = 100 * (degrees / STEP_ANGLE) * MICROSTEPPING; // 1.8 degree with no microstepping
    for (int i = 0; i < pulses_to_move; i++) {
        gpio_put(step_pin, 1);
        sleep_us(35);
        gpio_put(step_pin, 0);
        sleep_us(35);
    }

}
void move_motor(float distance) {
    uint dir_pin;
    uint step_pin;

    // axis = true for Y, false for X
    dir_pin = YDIR;
    step_pin = YSTEP;
    // move vertical by distance
        // set direction
    if (distance > 0) {
        printf("high\n");
        gpio_put(dir_pin, 1);
    }
    else {
        printf("low\n");
        gpio_put(dir_pin, 0);
    }

    distance = fabsf(distance) * 10;
    // step motor
    int pulses_per_rev = (360 / STEP_ANGLE) * MICROSTEPPING; // 1.8 degree with no microstepping
    int total_pulses = (int)((distance / LEAD_SCREW_PITCH) * pulses_per_rev);
    for (int i = 0; i < total_pulses; i++) {
        gpio_put(step_pin, 1);
        sleep_us(35);
        gpio_put(step_pin, 0);
        sleep_us(35);
    }
   //sleep_ms(1000);  
}

bool repeating_timer_callback(__unused struct repeating_timer *t) {
    
    if (candle_status()){
        pwm_set_chan_level(pwm_gpio_to_slice_num(SPEAKER_PWM), PWM_CHAN_B, SPEAKER_DUTY_CYCLE);
        extinguish_candle();
        pwm_set_chan_level(pwm_gpio_to_slice_num(SPEAKER_PWM), PWM_CHAN_B, 0);
        
    };
    printf("in timer callback\n");

    return true;
}

float prev_distance = 0.0f;

struct repeating_timer timer;

void light_candle() {
    // read US sensor for Y
    // move motor X by constant amount
    // enable PWM for gate driver to light candle
    // move motor Y by Y
    // hold 2 seconds
    // disable PWM
    // move motor Y back
    // move motor X back

    bool cancelled = cancel_repeating_timer(&timer);

    float y_distance = 0.0f;
    //top level check
    float top_y_dist = ultrasonic_reading();
    while (top_y_dist > HEIGHT_LIMIT) {
        printf("Height limit exceeded.\n");
        top_y_dist = ultrasonic_reading();
    }
    //sleep_ms(2000);
    move_motor(5.0f); // move to postion 2
    printf("Moved to position 2\n");
    float pos2_y_dist = ultrasonic_reading() + 5.0f;
    while (pos2_y_dist > HEIGHT_LIMIT) {
        printf("Height limit exceeded.\n");
        pos2_y_dist = ultrasonic_reading() + 5.0f;
    }

    move_servo(LIGHT_DUTY_CYCLE);
    //pwm_set_enabled(pwm_gpio_to_slice_num(GATE_PWM), true);
    pwm_set_chan_level(pwm_gpio_to_slice_num(GATE_PWM), PWM_CHAN_A, DUTY_CYCLE);
    sleep_ms(2000);
    if (top_y_dist < pos2_y_dist) {
        y_distance = top_y_dist;
        move_motor(top_y_dist - 5.0f - LIGHT_HEIGHT_OFFSET);
    }
    else {
        y_distance = pos2_y_dist;
        move_motor(pos2_y_dist - 5.0f - LIGHT_HEIGHT_OFFSET);
    }

    sleep_ms(1000);
    
    pwm_set_chan_level(pwm_gpio_to_slice_num(GATE_PWM), PWM_CHAN_A, 0);


    float temp = y_distance - LIGHT_HEIGHT_OFFSET;

    move_motor(-temp);
    
    while (!candle_status()) {
        printf("Candle is still not lit, trying again...\n");
        temp += 0.5f;
        //sleep_ms(1000);
        pwm_set_chan_level(pwm_gpio_to_slice_num(GATE_PWM), PWM_CHAN_A, DUTY_CYCLE);

        move_motor(temp);
        sleep_ms(1000);
        pwm_set_chan_level(pwm_gpio_to_slice_num(GATE_PWM), PWM_CHAN_A, 0);
        move_motor(-temp);
    }
    sleep_ms(2000);
    move_servo(NEUTRAL_DUTY_CYCLE);
    prev_distance = temp + LIGHT_HEIGHT_OFFSET;

    add_repeating_timer_ms(10000, repeating_timer_callback, NULL, &timer);
}

void extinguish_candle() {
    // read US sensor for Y
    // move motor X by constant amount
    // enable PWM for gate driver to extinguish candle
    // move motor Y by Y
    // hold 2 seconds
    // disable PWM
    // move motor Y back
    // move motor X back
    /*
    float y_distance = ultrasonic_reading() - SNUFF_HEIGHT_OFFSET;

    if (prev_distance != 0.0f) {
        y_distance = prev_distance;
    }
    */

    bool cancelled = cancel_repeating_timer(&timer);

    float y_distance = prev_distance - SNUFF_HEIGHT_OFFSET;
    
    move_servo(SNUFF_DUTY_CYCLE);
    sleep_ms(2000);
    move_motor(y_distance);
    sleep_ms(2000);
    move_motor(-5.0f);

    float temp = 5.0f;
    
    while (candle_status()) {
        printf("Candle is still lit, trying again...\n");
        temp += 0.5f;
        //sleep_ms(1000);
        move_motor(temp);
        sleep_ms(2000);
        move_motor(-temp);
    }

    move_motor(-(y_distance - temp));

    sleep_ms(1000);
    move_servo(NEUTRAL_DUTY_CYCLE);

    add_repeating_timer_ms(10000, repeating_timer_callback, NULL, &timer);

}


//
void move_servo(float duty_cycle) {
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, duty_cycle);
}

// Missing:
// - button read? if we use bigger button
// - buzzer
// - init timer but dont enable until light sequence


// add constraints to how far you can go down (no go past 16); base is static high
// save distance from lightng to compare when snuffing, maybe light affects distance reading
// if not turned on or off when told, have loop that tries again, adds some partial distance
