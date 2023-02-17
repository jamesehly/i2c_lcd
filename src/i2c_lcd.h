/**
 ****************************************************************************************
 *
 * @file    i2c_lcd.h
 * @version 1.0.1
 * @brief   I2C LCD driver header file.
 *
 * MIT License
 * 
 * Copyright (c) 2023 James Ehly
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ****************************************************************************************
 */

#ifndef _I2C_LCD_H_
#define _I2C_LCD_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include "user_periph_setup.h"

/**
 ****************************************************************************************
 * CONFIG DEFINES
 ****************************************************************************************
 */
#define LCD_DRVER_VER       "1.0.1"
#define LCD_ENTRY_INC       0x02
#define LCD_ENTRY_DEC       0x00
#define LCD_SHIFT_ON        0x01  
#define LCD_SHIFT_OFF       0x00
#define LCD_FONT_5x10       0x04
#define LCD_FONT_5x8        0x00
#define LCD_TWO_LINES       0x08
#define LCD_ONE_LINE        0x00
 
 /**
 ****************************************************************************************
 * INTERNAL DEFINES
 ****************************************************************************************
 */
#define MODE_4BIT           0
#define MODE_8BIT           1
#define INST_REGR           0x00 // 4bit instruction register select
#define DATA_REGR           0x01 // 4bit data register select 
#define NUM_LINES           I2C_LCD_NUM_LINES
#define NUM_COLMS           I2C_LCD_NUM_COLS

// SET DDRAM ADDRESS Command
#define LCD_SET_DDR_ADR_CMD 0x80
// SET CGRAM ADDRESS Command
#define LCD_SET_CGR_ADR_CMD 0x40

// Instruction register commands
#define LCD_CLEAR_CMD       0x01
#define LCD_HOME_CMD        0x02
#define LCD_ENTRY_MODE_CMD  0x04

// Display Commands
#define LCD_DISPLAY_CMD     0x08
#define LCD_DISPLAY_ON      0x04
#define LCD_CURSOR_ON       0x02
#define LCD_BLINK_ON        0x01

// Shift Commands
#define LCD_SHIFT_CMD       0x10
#define LCD_DISPLAY_SHIFT   0x08 // Shift off == cursor shift
#define LCD_SHIFT_RIGHT     0x04 // Right off == left shift

// Backlight States
#define LCD_BACKLIGHT_OFF 0x00
#define LCD_BACKLIGHT_ON  0x08

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
 /**
 ****************************************************************************************
 * Initialization function for i2c_lcd.
 *
 * Must be called after the GPIO pads have been set because it uses I2C run the LCD 
 * initialization sequence.  Place in user_periph_setup periph_init function, after 
 * GPIO_set_pad_latch_en function. Can also be placed in main loop or callbacks.
 * Initialization sequence is described in HD44780U datasheet pg. 46, fig 24 - 4-bit 
 * interface. See source file for initialization steps.
 ****************************************************************************************
 */
void i2c_lcd_init(void);

 /**
 ****************************************************************************************
 * Print an array of chars on the LCD screen
 *
 * @param[in] data Character data pointer
 * @param[in] length length of data to print
 ****************************************************************************************
 */
void i2c_lcd_print(uint8_t *data, uint8_t length);

 /**
 ****************************************************************************************
 * Set the cursor to col and row on the LCD screen
 *
 * @param[in] col column number, zero indexed
 * @param[in] row row number, zero indexed
 ****************************************************************************************
 */
void i2c_lcd_set_cursor(uint8_t col, uint8_t row);

 /**
 ****************************************************************************************
 * Clear the LCD screen
 ****************************************************************************************
 */
void i2c_lcd_clear(void);

 /**
 ****************************************************************************************
 * Clears a line to length.  
 * 
 * User needs to set the cursor using the i2c_lcd_set_cursor function prior to clearing
 * the line.  Note that this means you can partially clear a line by setting the cursor
 * at a position greater than zero.
 *
 * @param length length of the line to clear
 ****************************************************************************************
 */
void i2c_lcd_clear_line(uint8_t length);

 /**
 ****************************************************************************************
 * Turns the LCD backlight on
 ****************************************************************************************
 */
void i2c_lcd_backlight_on(void);

 /**
 ****************************************************************************************
 * Turns the LCD backlight off
 ****************************************************************************************
 */
void i2c_lcd_backlight_off(void);

 /**
 ****************************************************************************************
 * Shifts the contents of the screen to the right withought modifying the DDRAM
 ****************************************************************************************
 */
void i2c_lcd_shift_right(void);

 /**
 ****************************************************************************************
 * Shifts the contents of the screen to the left withought modifying the DDRAM
 ****************************************************************************************
 */
void i2c_lcd_shift_left(void);

 /**
 ****************************************************************************************
 * Places the cursor at col 0, row 0
 ****************************************************************************************
 */
void i2c_lcd_home(void);

 /**
 ****************************************************************************************
 * Turns off the LCD display (makes the text disappear).
 ****************************************************************************************
 */
void i2c_lcd_display_on(void);

 /**
 ****************************************************************************************
 * Turns on the LCD display (makes the text reappear).
 ****************************************************************************************
 */
void i2c_lcd_display_off(void);

 /**
 ****************************************************************************************
 * Starts blinking the cursor (typically a rectangle)
 ****************************************************************************************
 */
void i2c_lcd_blink_on(void);

 /**
 ****************************************************************************************
 * Stops the blinking cursor
 ****************************************************************************************
 */
void i2c_lcd_blink_off(void);

 /**
 ****************************************************************************************
 * Displays the cursor at the current position (typically an underline)
 ****************************************************************************************
 */
void i2c_lcd_cursor_on(void);

 /**
 ****************************************************************************************
 * Turns the cursor off
 ****************************************************************************************
 */
void i2c_lcd_cursor_off(void);

 /**
 ****************************************************************************************
 * Creates a custom character
 ****************************************************************************************
 */
void i2c_lcd_create_char(uint8_t,  uint8_t *charmap);

#endif // _I2C_LCD_H_
