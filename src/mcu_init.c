#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "config.h"
#include "mcu_init.h"


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