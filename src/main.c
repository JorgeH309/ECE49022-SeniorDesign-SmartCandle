#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "config.h"
#include "init.h"
#include "system_utils.h"


int main() {
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();
    /*
    while (true) {
        printf("Testing\n");
        sleep_ms(1000);
    }
    */
    // Intialize all componens
    button_gpio_init();
    us_gpio_init();
    motor_gpio_init();
    ir_gpio_init();
    gate_driver_pwm_init();
    speaker_pwm_init();
    servo_gpio_init();

    if (TEST_MODE == LIVE) {
        while (true) {
            if (gpio_get(BUTTON) == 1) {
                printf("Button pressed!\n");
                // button pressed
                // check IR sensor
                if (candle_status()) {
                    printf("Candle is lit, extinguishing...\n");
                    // candle is lit, extinguish
                    extinguish_candle();
                }
                else {
                    printf("Candle is not lit, lighting...\n");
                    // candle is not lit, light
                    light_candle();
                }
            }
        }
    }
    else if (TEST_MODE == DEBUG) {

        switch(DEBUG_DEVICE) {
            printf("Debug mode\n");
            case MOTOR:
                printf("Testing motor...\n");

                /*
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
                }*/

                while (true) {
                    move_motor(5.0f);
                    sleep_ms(2000);
                    move_motor(-5.0f);
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
                    float distance = ((duration * 0.0343f) / 2.0f);;

                    printf("Distance: %.2f cm\n", distance);

                    sleep_ms(500);
                }
                break;
            case SERVO:
                printf("Testing servo...\n");


                while (true) {
                    move_servo(LIGHT_DUTY_CYCLE);
                    sleep_ms(1000);
                    move_servo(NEUTRAL_DUTY_CYCLE);
                    sleep_ms(1000);
                    move_servo(SNUFF_DUTY_CYCLE);
                    sleep_ms(1000);
                    move_servo(NEUTRAL_DUTY_CYCLE);
                    sleep_ms(1000);
                }
                uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

                while (true) {
                    for (int i = 500; i < 2500; i++) {
                        pwm_set_chan_level(slice_num, PWM_CHAN_A, i);
                        //pwm_set_enabled(slice_num, true);
                        sleep_ms(20);
                    }
                    sleep_ms(1000);

                    for (int i = 2500; i < 500; i--) {
                        pwm_set_chan_level(slice_num, PWM_CHAN_A, i);
                        //pwm_set_enabled(slice_num, true);
                        sleep_ms(20);
                    }
                    sleep_ms(1000);
                }
                break;
            case HORIZ_STEPPER:

                while (true) {
                    move_motor(10.0f);
                    sleep_ms(2000);
                    move_motor(-10.0f);
                    sleep_ms(2000);
                }
                /*
                printf("Testing horizontal stepper...\n");
                while (true) {
                    move_stepper_horiz(LIGHTER, true);
                    sleep_ms(2000);
                    move_stepper_horiz(LIGHTER, false);
                    sleep_ms(2000);
                    move_stepper_horiz(SNUFFER, true);
                    sleep_ms(2000);
                    move_stepper_horiz(SNUFFER, false);
                    sleep_ms(2000);
                }
                */
                break;
        }
    }

    else if (TEST_MODE == SUB_DEMO) {
        printf("Running sub-demo...\n");
        while (true) {
            //float y_distance = ultrasonic_reading() * 100;
            //printf("Retrieved distance: %.2f mm\n", y_distance);
            if (gpio_get(BUTTON) == 1) {

                if(candle_status()) {
                    printf("Candle is lit\n");
                }
                else {
                    printf("Candle is not lit.\n");
                }
                float distance_read = ultrasonic_reading();
                printf("Finished US reading\n");
                float y_distance = 100.0f; //mm
                
                // axis = true for Y, false for X
                move_motor(-y_distance); 
                sleep_ms(3000);
                
                // axis is false for X
                //move_motor(LIGHT); move horizontal with servo

                printf("Almost done\n");
                move_motor(+y_distance);
                sleep_ms(2000);
                //move_motor(-LIGHT, false); move horizontal with servo

                printf("Done\n");

            }
        }
    }
}
