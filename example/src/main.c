/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief I2C EEPROM example
 *
 * Copyright (C) 2012-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdio.h>
#include <stdint.h>
#include "arch_system.h"
#include "user_periph_setup.h"
#include "gpio.h"
#include "uart.h"
#include "systick.h"
#include "uart_utils.h"
#include "i2c_lcd.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define BUFFER_SIZE             (256)

/*
 * VARIABLE DEFINITIONS
 ****************************************************************************************
 */

uint8_t wr_data[BUFFER_SIZE];
uint8_t rd_data[BUFFER_SIZE];

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief I2C test function
 ****************************************************************************************
 */
void i2c_test(void);

/**
 ****************************************************************************************
 * @brief  Main routine of the I2C EEPROM example
 ****************************************************************************************
 */
int main (void)
{
    system_init();
    i2c_test();
    while(1);
}

void wait() {
    systick_wait(1000000);
}

void display_message(uint8_t *message, uint8_t length, uint8_t row) {
    i2c_lcd_set_cursor(0, row);
    i2c_lcd_print(message, length);
}

void display_start_screen() {
    uint8_t buffer_1[24] = "    HELLO LCD WORLD!    ";
    uint8_t buffer_2[24] = "1234567890!@#$%^&*()_+?=";
    display_message(buffer_1, 24, 0);
    display_message(buffer_2, 24, 1);
}

void i2c_test(void)
{
    uint8_t bufferON[2] = "On";
    uint8_t bufferOFF[3] = "Off";

    // Display Hello World
    display_start_screen();
    wait();
    
    // Backlight off/on
    i2c_lcd_clear();
    uint8_t buffer1[16] = "Backlight OFF/ON";
    display_message(buffer1, 16, 0);
    wait();
    i2c_lcd_backlight_off();
    wait();
    i2c_lcd_backlight_on();
    wait();
    
    // Display off/on
    i2c_lcd_clear();
    uint8_t buffer2[14] = "Display OFF/ON";
    display_message(buffer2, 14, 0);
    wait();
    i2c_lcd_display_off();
    wait();
    i2c_lcd_display_on();
    wait();
    
    // Cursor off/on 
    i2c_lcd_clear();
    uint8_t buffer3[6] = "Cursor";
    display_message(buffer3, 6, 0);
    display_message(bufferON, 2, 1);
    wait();
    i2c_lcd_cursor_on();
    wait();
    wait();
    display_message(bufferOFF, 3, 1);
    i2c_lcd_cursor_off();
    wait();
    wait();
    
    // Blink off/on 
    i2c_lcd_clear();
    uint8_t buffer4[5] = "Blink";
    display_message(buffer4, 5, 0);
    display_message(bufferON, 2, 1);
    wait();
    i2c_lcd_blink_on();
    wait();
    wait();
    display_message(bufferOFF, 3, 1);
    i2c_lcd_blink_off();
    wait();
    wait();
    
    // Shift right/left 
    i2c_lcd_clear();
    uint8_t buffer5[16] = "Shift RIGHT/LEFT";
    display_message(buffer5, 16, 0);
    wait();
    i2c_lcd_shift_right();
    i2c_lcd_shift_right();
    i2c_lcd_shift_right();
    wait();
    i2c_lcd_shift_left();
    i2c_lcd_shift_left();
    i2c_lcd_shift_left();
    wait();
}
