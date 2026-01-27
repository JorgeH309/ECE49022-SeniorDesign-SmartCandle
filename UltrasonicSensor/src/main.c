#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// just use pin numbers directly on board
const int TRIG = 28;
const int ECHO = 27;

void us_gpio_init() {
    //TRIG pin, output
    gpio_set_function(TRIG, GPIO_FUNC_SIO);
    gpio_set_dir(TRIG, GPIO_OUT);

    //ECHO pin, input
    gpio_set_function(ECHO, GPIO_FUNC_SIO);
    gpio_set_dir(ECHO, GPIO_IN);
}
int main() {
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();
    

    us_gpio_init();
    
    printf("Initialized\n");
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
        float distance = (duration * 0.0343f) / 2.0f;

        printf("Distance: %.2f cm\n", distance);

        sleep_ms(2000);
    }

    


    // An infinite loop is necessary to 
    // ensure control flow remains with user.
    for(;;);

    // Never reached.
    return 0;
}