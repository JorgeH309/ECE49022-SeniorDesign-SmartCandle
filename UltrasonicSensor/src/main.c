#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// just use pin numbers directly on board
const int TRIG = 28;
const int ECHO = 27;
const int DIR = 6;
const int STEP = 5;


bool MOTOR = true;
// true for motor

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
    gpio_set_function(DIR, GPIO_FUNC_SIO);
    gpio_set_dir(DIR, GPIO_OUT);

    //STEP pin, output
    gpio_set_function(STEP, GPIO_FUNC_SIO);
    gpio_set_dir(STEP, GPIO_OUT);
}
int main() {
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();
    
    if (MOTOR) {
        motor_gpio_init();
    }
    else {
        us_gpio_init();
    }
    
    printf("Initialized\n");
    
    while (true) {
        if (MOTOR) {
            // set direction
            gpio_put(DIR, 1);
            // step motor
            for (int i = 0; i < 3200; i++) {
                gpio_put(STEP, 1);
                sleep_us(500);
                gpio_put(STEP, 0);
                sleep_us(500);
            }
            sleep_ms(2500);            
            
            gpio_put(DIR, 0);
            // step motor
            for (int i = 0; i < 3200; i++) {
                gpio_put(STEP, 1);
                sleep_us(500);
                gpio_put(STEP, 0);
                sleep_us(500);
            }
            sleep_ms(1000);            
            

        }
        else {
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
    }

    


    // An infinite loop is necessary to 
    // ensure control flow remains with user.
    for(;;);

    // Never reached.
    return 0;
}