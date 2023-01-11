/**
 ****************************************************************************************
 *
 * @file i2c_lcd.h
 *
 * @brief I2C LCD driver header file.
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
 * Print an array of chars on the lcd screen
 *
 * Must be called after the GPIO pads have been set because it uses I2C run the LCD 
 * initialization sequence.  Initialization sequence is described in HD44780U datasheet
 * pg. 46 (fig 24 - 4-bit interface). See source file for initialization steps
 *
 * @param[in] data Character data pointer
 * @param[in] length length of data to print/
 * @note if length is ommitted or is longer than actual data size, then the actual data
         size will be used
 ****************************************************************************************
 */
void i2c_lcd_print(uint8_t *data, uint8_t length);
void i2c_lcd_set_cursor(uint8_t col, uint8_t row);
void i2c_lcd_clear(void);
void i2c_lcd_clear_line(uint8_t length);
void i2c_lcd_backlight_on(void);
void i2c_lcd_backlight_off(void);
void i2c_lcd_shift_right(void);
void i2c_lcd_shift_left(void);
void i2c_lcd_home(void);
void i2c_lcd_display_on(void);
void i2c_lcd_display_off(void);
void i2c_lcd_blink_on(void);
void i2c_lcd_blink_off(void);
void i2c_lcd_cursor_on(void);
void i2c_lcd_cursor_off(void);

#endif // _I2C_LCD_H_
