/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief I2C LCD example
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

/*
 * VARIABLE DEFINITIONS
 ****************************************************************************************
 */
uint16_t sequence_count = 0;
uint8_t ship_pos = 0;
int ship_dir = 1;
uint8_t fire_seq = 0;
uint8_t alien_pos = I2C_LCD_NUM_COLS - 1;
int alien_dir = -1;
uint8_t aliens[16] = {0x00, ' ', 0x01, ' ', 0x02, ' ', 0x03, 0x04, 0x05, 0x06, ' ', 0x02, ' ', 0x01, ' ', 0x00};

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
    uint8_t buffer_v1[5] = LCD_DRVER_VER;
    uint8_t buffer_v2[8] = "Version ";
    uint8_t buffer_1[15]  = "DA1453x I2C_LCD";
    
    display_message(buffer_1, 15, 0);
    i2c_lcd_set_cursor(0, 1);
    i2c_lcd_print(buffer_v2, 8);
    i2c_lcd_print(buffer_v1, 5);
}

void end_demo() {
    i2c_lcd_clear();
    uint8_t buffer_1[3]  = "THE";
    uint8_t buffer_2[3]  = "END";
    i2c_lcd_set_cursor(I2C_LCD_NUM_COLS / 2 - 2 , 0);
    i2c_lcd_print(buffer_1, 3);
    i2c_lcd_set_cursor(I2C_LCD_NUM_COLS / 2 - 1 , 1);
    i2c_lcd_print(buffer_2, 3);
}

void display_all_chars() {
    uint8_t row = 1;
    for(uint16_t i = 0;i < 256; i++) {
        if(i % I2C_LCD_NUM_COLS == 0) {
            row = row == 0 ? 1 : 0;
            i2c_lcd_set_cursor(0, row);
        }
        uint8_t c[1] = {i};
        i2c_lcd_print(c, 1);
        systick_wait(250000);
    }    
}

void create_characters() {
  bool isEven = sequence_count % 2 ==0;

  uint8_t a1[7] = {
      0xFF,                     // OOOOO
      0x15,                     // O O O
      0xFF,                     // OOOOO
      isEven ? 0x11 : 0x0A,     //  O O 
      isEven ? 0x0A : 0x11,     // O   O
      0x00,
      0x00,
    };
    uint8_t a2[7] = {
      0xFF,                     // OOOOO
      0x15,                     // O O O
      isEven ? 0xFF : 0x0E,     //  OOO 
      0x15,                     // O O O
      isEven ? 0x0A : 0x04,     //   O
      0x00,
      0x00,
    };
    uint8_t a3[7] = {
      0x0E,                     //  OOO
      0x1B,                     // OO OO
      0xFF,                     // OOOOO 
      0x11,                     // O   O
      isEven ? 0x11 : 0x0A,     //  O O
      0x00,
      0x00,
    };
    
    uint8_t ss1[7] = {
      0x10,                     // O   
      0x17,                     // O OOO
      0x19,                     // OO  O 
      0x19,                     // OO  O
      0x0F,                     //  OOOO
      0x03,                     //    OO
      0x00,
    };
    
    uint8_t ss2[7] = {
      0x00,                     // 
      0xFF,                     // OOOOO
      0x19,                     // OO  O
      0xFF,                     // OOOOO 
      0xFF,                     // OOOOO
      isEven ? 0x12 : 0x14,     // O O
      isEven ? 0x04 : 0x02,     //    O
    };
    
    uint8_t ss3[7] = {
      0x00,                     // OOOOO    
      0xFF,                     // OOOOO
      0x13,                     // O  OO
      0xFF,                     // OOOOO
      0xFF,                     // OOOOO
      isEven ? 0x09 : 0x05,     //   O O
      isEven ? 0x04 : 0x08,     //  O  
    };
    
    
    uint8_t ss4[7] = {
      0x01,                     //     O
      0x1D,                     // OOO O
      0x13,                     // O  OO 
      0x13,                     // O  OO
      0x1E,                     // OOOO
      0x18,                     // OO
      0x00,
    };
    
    
    uint8_t s1[7] = {
      fire_seq == 5 ? 0x11 : 0x00,      //   
      fire_seq == 4 ? 0x15 : 0x04,      //   O
      fire_seq == 3 ? 0x15 : 0x04,      //   O 
      0x0E,                             //  O O
      0x1B,                             // OO OO
      0xFF,                             // OOOOO
      0x15,                             // O O O
    };
    
    i2c_lcd_create_char(0, a1);
    i2c_lcd_create_char(1, a2);
    i2c_lcd_create_char(2, a3);
    i2c_lcd_create_char(3, ss1);
    i2c_lcd_create_char(4, ss2);
    i2c_lcd_create_char(5, ss3);
    i2c_lcd_create_char(6, ss4);
    i2c_lcd_create_char(7, s1);
}

static void run_character_sequence(void) {
    if(sequence_count > I2C_LCD_NUM_COLS - 1) {
        i2c_lcd_set_cursor(0, 0);
        i2c_lcd_clear_line(I2C_LCD_NUM_COLS);
    }
    
    i2c_lcd_set_cursor(alien_pos, 0);
    i2c_lcd_print(aliens, sequence_count < 16 ? sequence_count + 1 : 16);
    
    if(sequence_count >= 16 && sequence_count < I2C_LCD_NUM_COLS) {
        i2c_lcd_set_cursor(alien_pos + 16, 0);
        i2c_lcd_clear_line(sequence_count - 16 + 1);
    }
    
    i2c_lcd_set_cursor(0, 1);
    i2c_lcd_clear_line(I2C_LCD_NUM_COLS);
    i2c_lcd_set_cursor(ship_pos, 1);
    uint8_t ship[1] = {0x07};
    i2c_lcd_print(ship, 1);
    create_characters();
    ship_pos += ship_dir;
    alien_pos += alien_dir;
    sequence_count++;
    fire_seq++;
    if(ship_pos == I2C_LCD_NUM_COLS - 1) {
        ship_dir = -1;
    }
    if(ship_pos == 0) {
        ship_dir = 1;
    }
    if(fire_seq == 6) {
        fire_seq = 0;
    }
    if(alien_pos == 0) {
        alien_dir = 1;
    }
    if(alien_pos == I2C_LCD_NUM_COLS - 16) {
        alien_dir = -1;
    }
    if(sequence_count < I2C_LCD_NUM_COLS * 2) {
        systick_start(1000000, 1);
    } else {
        end_demo();
    }
}

void i2c_test(void)
{
    uint8_t bufferON[2] = "On";
    uint8_t bufferOFF[3] = "Off";
    
    i2c_lcd_clear();
    wait();
    
    // Display Hello World
    display_start_screen();
    wait();
    
    // Display All Characters
    display_all_chars();
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
    
    // Create Custom Characters
    i2c_lcd_clear();
    uint8_t buffer7[17] = "Custom Characters";
    wait();
    display_message(buffer7, 17, 0);
    create_characters();
    systick_register_callback(run_character_sequence);
    systick_start(1000000, 1);
}

