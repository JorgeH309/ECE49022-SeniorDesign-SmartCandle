#include <stdio.h>
#include "pico/stdlib.h"

//////////////////////////////////////////////////////////////////////////////

const char* username = "herna647";

//////////////////////////////////////////////////////////////////////////////

void autotest();


void init_output_helper(uint gpio) {
    bool out = true; // To make them outputs
    bool value = false; // make them output low
    gpio_function_t fn = GPIO_FUNC_SIO;
    //set direction to output
    uint32_t mask = 1u << (gpio & 0x1fu);
    /* Purpose only lowest 5 bits are used for range (0-31)*/
    if (gpio < 32) {
        if (out) {
            sio_hw->gpio_oe_set = mask;
        } else {
            sio_hw->gpio_oe_clr = mask;
        }
    } else {
        if (out) {
            sio_hw->gpio_hi_oe_set = mask;
        } else {
            sio_hw->gpio_hi_oe_clr = mask;
        }
    }

    //set output to low
    mask = 1ul << (gpio & 0x1fu);
    if (gpio < 32) {
        if (value) {
            sio_hw->gpio_set = mask;
        } else {
            sio_hw->gpio_clr = mask;
        }
    } else {
        if (value) {
            sio_hw->gpio_hi_set = mask;
        } else {
            sio_hw->gpio_hi_clr = mask;
        }
    }

    check_gpio_param(gpio);
    invalid_params_if(HARDWARE_GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
    // Set input enable on, output disable off
    hw_write_masked(&pads_bank0_hw->io[gpio],
                   PADS_BANK0_GPIO0_IE_BITS,
                   PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
    );
    // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
    // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
    io_bank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
#if HAS_PADS_BANK0_ISOLATION
    // Remove pad isolation now that the correct peripheral is in control of the pad
    hw_clear_bits(&pads_bank0_hw->io[gpio], PADS_BANK0_GPIO0_ISO_BITS);
#endif

}

void init_outputs() {

    init_output_helper(22);
    init_output_helper(23);
    init_output_helper(24);
    init_output_helper(25);

    
}

void init_inputs() {
    bool out = true; // To make them input
    gpio_function_t fn = GPIO_FUNC_SIO;
    uint gpio = 21;
    //set direction to output
    uint32_t mask = 1u << (gpio & 0x1fu);
    sio_hw->gpio_oe_clr = mask;
    
    check_gpio_param(gpio);
    invalid_params_if(HARDWARE_GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
    // Set input enable on, output disable off
    hw_write_masked(&pads_bank0_hw->io[gpio],
                   PADS_BANK0_GPIO0_IE_BITS,
                   PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
    );
    // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
    // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
    io_bank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
#if HAS_PADS_BANK0_ISOLATION
    // Remove pad isolation now that the correct peripheral is in control of the pad
    hw_clear_bits(&pads_bank0_hw->io[gpio], PADS_BANK0_GPIO0_ISO_BITS);
#endif

    gpio = 26;
    mask = 1u << (gpio & 0x1fu);
    sio_hw->gpio_oe_clr = mask;

    check_gpio_param(gpio);
    invalid_params_if(HARDWARE_GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
    // Set input enable on, output disable off
    hw_write_masked(&pads_bank0_hw->io[gpio],
                   PADS_BANK0_GPIO0_IE_BITS,
                   PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
    );
    // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
    // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
    io_bank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
#if HAS_PADS_BANK0_ISOLATION
    // Remove pad isolation now that the correct peripheral is in control of the pad
    hw_clear_bits(&pads_bank0_hw->io[gpio], PADS_BANK0_GPIO0_ISO_BITS);
#endif


    
}

void init_keypad() {
    
    gpio_function_t fn = GPIO_FUNC_SIO;

    int rows[4] = {2, 3, 4, 5};
    int cols[4] = {6, 7, 8, 9};

    for(int i=0; i <4; i++) {
        int gpio = rows[i];
        //set direction to input
        uint32_t mask = 1u << (gpio & 0x1fu);
        sio_hw->gpio_oe_clr = mask;
        
        check_gpio_param(gpio);
        invalid_params_if(HARDWARE_GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
        // Set input enable on, output disable off
        hw_write_masked(&pads_bank0_hw->io[gpio],
                    PADS_BANK0_GPIO0_IE_BITS,
                    PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
        );
        // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
        // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
        io_bank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    #if HAS_PADS_BANK0_ISOLATION
        // Remove pad isolation now that the correct peripheral is in control of the pad
        hw_clear_bits(&pads_bank0_hw->io[gpio], PADS_BANK0_GPIO0_ISO_BITS);
    #endif

    }

    for (int i=0;i < 4; i++) {
        int gpio = cols[i];
        uint32_t mask = 1u << (gpio & 0x1fu);
        sio_hw->gpio_oe_set = mask;
        sio_hw->gpio_clr = mask;

        check_gpio_param(gpio);
        invalid_params_if(HARDWARE_GPIO, ((uint32_t)fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB) & ~IO_BANK0_GPIO0_CTRL_FUNCSEL_BITS);
        // Set input enable on, output disable off
        hw_write_masked(&pads_bank0_hw->io[gpio], //grabs GPIOX register
                    PADS_BANK0_GPIO0_IE_BITS,
                    PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS
        );
        // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
        // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
        io_bank0_hw->io[gpio].ctrl = fn << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB; // grabs GPIOX_CTRL register
    #if HAS_PADS_BANK0_ISOLATION
        // Remove pad isolation now that the correct peripheral is in control of the pad
        hw_clear_bits(&pads_bank0_hw->io[gpio], PADS_BANK0_GPIO0_ISO_BITS);
    #endif

    }

}



int main() {
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();
    


    // Leave commented until you actually need it.
    // Can take significantly longer to upload code when uncommented.
    autotest();
    
    ////////////////////////////////////
    // All your code goes below.
    
    init_outputs();
    init_inputs();
    init_keypad();
    /*
    int led[4] = {22, 23, 24, 25};

    int gpio = 0;
    bool on = true;
    while(true) {
    
        if (gpio >= 4){
            gpio = 0;  
            on = !on;
        }

        uint32_t mask = 1ul << (led[gpio] & 0x1fu);
        if (led[gpio] < 32) {
            if (on) {
                sio_hw->gpio_set = mask;
            } else {
                sio_hw->gpio_clr = mask;
            }
        } else {
            if (on) {
                sio_hw->gpio_hi_set = mask;
            } else {
                sio_hw->gpio_hi_clr = mask;
            }
        }
        sleep_ms(500);
        gpio++;


    }
    

    int led[4] = {22, 23, 24, 25};

    while (true) {
        
        uint32_t mask = 1u << (21 & 0x1fu);
        bool gpio21 = (sio_hw->gpio_in & mask) != 0;

        mask = 1u << (26 & 0x1fu);
        bool gpio26 = (sio_hw->gpio_in & mask) != 0;


        if (gpio21) {
            for (int gpio=0; gpio < 4; gpio++) {
                mask = 1ul << (led[gpio] & 0x1fu);
                sio_hw->gpio_set = mask;
            }
            gpio21 = !gpio21;
        }
        else if (gpio26) {
            for (int gpio=0; gpio < 4; gpio++) {
                mask = 1ul << (led[gpio] & 0x1fu);
                sio_hw->gpio_clr = mask;
            }
            gpio26 = !gpio26;
        }
        
        sleep_ms(10);
    }
    */
   int cols[4] = {6, 7, 8, 9};
   int rows[4] = {2, 3, 4, 5};

    int led[4] = {25, 24, 23, 22};

   while (true) {

    for (int i = 0; i < 4; i++) {
        uint32_t mask = 1ul << (cols[i] & 0x1fu);
        sio_hw->gpio_set = mask;

        sleep_ms(10);

        mask = 1u << (rows[i] & 0x1fu);
        bool button = (sio_hw->gpio_in & mask) != 0;
        
        if (!button) {
            mask = 1ul << (led[i] & 0x1fu);
            sio_hw->gpio_clr = mask;
        } else {
            mask = 1ul << (led[i] & 0x1fu);
            sio_hw->gpio_set = mask;
        }
        mask = 1ul << (cols[i] & 0x1fu);
        sio_hw->gpio_clr = mask;

    }

   }
    // All your code goes above.
    ////////////////////////////////////

    // An infinite loop is necessary to 
    // ensure control flow remains with user.
    for(;;);

    // Never reached.
    return 0;
}