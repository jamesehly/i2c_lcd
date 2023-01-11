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

void user_display_message(uint8_t *message, uint8_t length, uint8_t row) {
    i2c_lcd_set_cursor(0, row);
    i2c_lcd_print(message, length);
}

void user_display_start_screen() {
    uint8_t buffer_1[24] = "    HELLO LCD WORLD!    ";
    uint8_t buffer_2[24] = "1234567890!@#$%^&*()_+?=";
    user_display_message(buffer_1, 24, 0);
    user_display_message(buffer_2, 24, 1);
}

void i2c_test(void)
{
    user_display_start_screen();
    wait();
    i2c_lcd_backlight_off();
    wait();
    i2c_lcd_backlight_on();
    wait();
    i2c_lcd_display_off();
    wait();
    i2c_lcd_display_on();
}
