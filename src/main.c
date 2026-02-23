#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "config.h"


// *********** Peripheral initialization functions *********** //

void us_gpio_init() {
    //TRIG pin, output
    gpio_set_function(TRIG, GPIO_FUNC_SIO);
    gpio_set_dir(TRIG, GPIO_OUT);

    //ECHO pin, input
    gpio_set_function(ECHO, GPIO_FUNC_SIO);
    gpio_set_dir(ECHO, GPIO_IN);
}

void motor_gpio_init() {
    //DIR pin, output
    gpio_set_function(XDIR, GPIO_FUNC_SIO);
    gpio_set_dir(XDIR, GPIO_OUT);

    //STEP pin, output
    gpio_set_function(XSTEP, GPIO_FUNC_SIO);
    gpio_set_dir(XSTEP, GPIO_OUT);


        //DIR pin, output
    gpio_set_function(YDIR, GPIO_FUNC_SIO);
    gpio_set_dir(YDIR, GPIO_OUT);

    //STEP pin, output
    gpio_set_function(YSTEP, GPIO_FUNC_SIO);
    gpio_set_dir(YSTEP, GPIO_OUT);
}

void ir_gpio_init() {
    // IR sensor pin, input
    gpio_set_function(IR_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(IR_PIN, GPIO_IN);
}

void button_gpio_init() {
    // button pin,
    gpio_set_function(BUTTON, GPIO_FUNC_SIO);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
}

void gate_driver_pwm_init() {
    
    gpio_set_function(GATE_PWM, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(GATE_PWM);
    pwm_set_clkdiv(slice_num, CLOCK_DIVIDER);
    pwm_set_wrap(slice_num, PERIOD - 1);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, DUTY_CYCLE);

    //pwm_set_enabled(slice_num, true);

}

// *********** Helper functions *********** //

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
    bool ir_count = 0;
    for (int i = 0; i < 20; i++) {
        if (gpio_get(IR_PIN) == 1) {
            ir_count++;
        }
        sleep_ms(10);
    }
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
int main() {
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();
    
    // initiate button
    button_gpio_init();
    // initiate ultrasonic sensor
    us_gpio_init();
    // initiate motors
    motor_gpio_init();
    // initiate IR sensor pin
    ir_gpio_init();
    // initiate gate driver interface
    gate_driver_pwm_init();

    if (TEST_MODE == LIVE) {
        while (true) {
            if (gpio_get(BUTTON) == 0) {
                // button pressed
                // check IR sensor
                if (candle_status()) {
                    // candle is lit, extinguish
                    extinguish_candle();
                }
                else {
                    // candle is not lit, light
                    light_candle();
                }
            }
        }
    }
    else if (TEST_MODE == DEBUG) {

        switch(DEBUG_DEVICE) {
            case MOTOR:
                printf("Testing motor...\n");
                int rotations = 5;
                while (true) {
                    // set direction
                    gpio_put(YDIR, 1);
                    // step motor
                    for (int i = 0; i < (rotations * 3200); i++) {
                        gpio_put(YSTEP, 1);
                        sleep_us(35);
                        gpio_put(YSTEP, 0);
                        sleep_us(35);
                    }
                    sleep_ms(2000);            
                    
                    gpio_put(YDIR, 0);
                    // step motor
                    for (int i = 0; i <(rotations* 3200); i++) {
                        gpio_put(YSTEP, 1);
                        sleep_us(35);
                        gpio_put(YSTEP, 0);
                        sleep_us(35);
                    }
                    sleep_ms(2000);  
                }

                break;

            case ULTRASONIC:
                printf("Testing ultrasonic sensor...\n");
                while (true) {
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
                    float distance = ((duration * 0.0343f) / 2.0f) / 2.54f;;

                    printf("Distance: %.2f in\n", distance);

                    sleep_ms(500);
                }
                break;
            case SERVO:
                printf("Testing servo...\n");
                while (true) {
                    float pulse_width =10.0f;
                    for (int i = 0; i < 4; i++) {
                        //pulse_width += 0.1f;
                        //for (int i = 0; i < 10; i++) {
                            gpio_put(YSTEP, 1);
                            sleep_ms(pulse_width);
                            gpio_put(YSTEP, 0);
                            sleep_ms(20 - pulse_width);
                        //}

                    }

                    sleep_ms(2000);

                }
                break;

        }
    }

    else if (TEST_MODE == SUB_DEMO) {
        printf("Running sub-demo...\n");
        while (true) {
            //float y_distance = ultrasonic_reading() * 100;
            //printf("Retrieved distance: %.2f mm\n", y_distance);
            
            float y_distance = 100.0f;
            move_vertical(-y_distance);

            sleep_ms(20000);

            move_vertical(y_distance);

            sleep_ms(2000);

        }
       

    }
}
