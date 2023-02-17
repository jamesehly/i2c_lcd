/**
 ********************************************************************************
 *
 * @file i2c_lcd.c
 *
 * @brief I2C LCD driver for PCF8574 compatible LCD 'backpack' modules.
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
 ********************************************************************************
 */

#include "i2c.h"
#include "user_periph_setup.h"
#include "systick.h"
#include "i2c_lcd.h"

/**
 ****************************************************************************************
 * DEFAULT CONFIG
 ****************************************************************************************
 */
 
#ifndef I2C_LCD_ADDRESS
#define I2C_LCD_ADDRESS     0x27
#endif

#ifndef I2C_LCD_NUM_LINES
#define I2C_LCD_NUM_LINES   LCD_TWO_LINES
#endif

#ifndef I2C_LCD_NUM_COLS
#define I2C_LCD_NUM_COLS    16
#endif

#ifndef I2C_LCD_ENTRY_MODE
#define I2C_LCD_ENTRY_MODE  LCD_ENTRY_INC
#endif

#ifndef I2C_LCD_SHIFT_MODE
#define I2C_LCD_SHIFT_MODE  LCD_SHIFT_OFF
#endif

#ifndef I2C_LCD_FONT_MODE
#define I2C_LCD_FONT_MODE   LCD_FONT_5x8
#endif

// Private state variables
uint8_t _backlight          = 0x00;
uint8_t _display            = LCD_DISPLAY_ON;
uint8_t _cursor             = 0x00;
uint8_t _blink              = 0x00;
uint8_t _shift_display      = 0x00;
uint8_t _shift_right        = LCD_SHIFT_RIGHT;
uint8_t _entry_mode         = I2C_LCD_ENTRY_MODE;
uint8_t _shift_mode         = I2C_LCD_SHIFT_MODE;

/*-------------------------------------------------------------------------- */
/* Private Function Declarations                                             */ 
/*---------------------------------------------------------------------------*/
uint16_t _i2c_lcd_send(
    const uint8_t *data, 
    uint16_t len, 
    uint8_t mode, 
    uint8_t rs
    );
static void _i2c_lcd_set_address(void);
void _i2c_send_and_wait_8bit(uint8_t byte, uint32_t us);
uint8_t _i2c_lcd_display_cmd(void);
void _i2c_lcd_init(void);
void _i2c_lcd_command(const uint8_t byte);
uint8_t _i2c_lcd_entry_mode_cmd(void);

/*-------------------------------------------------------------------------- */
/* LCD API Functions                                                         */ 
/*---------------------------------------------------------------------------*/

/**
 ******************************************************************************
 * This is the software initialization procedure as described in the 
 * HD44780U Instruction manual, pg. 46
 ******************************************************************************
 */
void i2c_lcd_init() {
    // Wait 50ms for power up (systick is in microseconds 50000us = 50ms)
    systick_wait(50000);
    
    // Set the address of the LCD
    _i2c_lcd_set_address();
    
    // Send command to turn off the backlight (this step is ommitted in the manual)
    uint8_t data[1] = {_backlight};
    i2c_abort_t abort_code;
    i2c_master_transmit_buffer_sync(data, 1, &abort_code, I2C_F_ADD_STOP);
    systick_wait(200);
    
    // 8bit mode function set called 3x
    _i2c_send_and_wait_8bit(0x30, 4500); // send and wait 4.5ms
    _i2c_send_and_wait_8bit(0x30, 4500); // send and wait 4.5ms
    _i2c_send_and_wait_8bit(0x30, 200);  // send and wait 200us
    
    // Send the command to switch to 4bit mode (in 8bit mode)
    uint8_t mode_4bit[1] = {0x20};
    _i2c_lcd_send(mode_4bit, 1, MODE_8BIT, INST_REGR);
    
    // Now we are in 4bit mode. Not we are not checking the BF flag so wait times
    // are hard coded according to the datasheet
    uint8_t data_4bit[2] = { 
        0x28, // function set - 2 lines and 5x8 char set
        0x0C, // display off
     };
    _i2c_lcd_send(data_4bit, 2, MODE_4BIT, INST_REGR);
    
    // Clear command takes longer than a normal command
    i2c_lcd_clear();
    
    // Entry mode set is the final instruction
    _i2c_lcd_command(_i2c_lcd_entry_mode_cmd()); // entry mode set
    
    // This is not in the instructions but I figure its a nice thing to do.
    i2c_lcd_backlight_on();
}

void i2c_lcd_print(uint8_t *data, uint8_t length) {
    _i2c_lcd_send(data, length, MODE_4BIT, DATA_REGR);
}

void i2c_lcd_backlight_off() {
    _backlight = LCD_BACKLIGHT_OFF;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
}

void i2c_lcd_backlight_on() {
    _backlight = LCD_BACKLIGHT_ON;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
}

void i2c_lcd_clear() {
    _i2c_lcd_command(0x01);
    systick_wait(2000);
}

void i2c_lcd_clear_line(uint8_t length) {
    // 40 space buffer because 40 is the max length of any display we'll use
    uint8_t buffer[] = "                                        ";
    _i2c_lcd_send(buffer, length, MODE_4BIT, DATA_REGR);
}

void i2c_lcd_home() {
    _i2c_lcd_command(0x02);
    systick_wait(2000);
}

void i2c_lcd_set_cursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > NUM_LINES ) {
		row = NUM_LINES-1;    // we count rows starting w/0
	}
	_i2c_lcd_command(LCD_SET_DDR_ADR_CMD | (col + row_offsets[row]));
}

void i2c_lcd_shift_right() {
    _i2c_lcd_command(0x1C);
};

void i2c_lcd_shift_left(){
    _i2c_lcd_command(0x18);
};

void i2c_lcd_display_on() {
    _display = LCD_DISPLAY_ON;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
};
void i2c_lcd_display_off() {
    _display = 0x00;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
};
void i2c_lcd_blink_on() {
    _blink = LCD_BLINK_ON;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
};
void i2c_lcd_blink_off() {
    _blink = 0x00;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
};
void i2c_lcd_cursor_on() {
    _cursor = LCD_CURSOR_ON;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
};
void i2c_lcd_cursor_off() {
    _cursor = 0x00;
    _i2c_lcd_command(_i2c_lcd_display_cmd());
    systick_wait(200);
};
void i2c_lcd_create_char(uint8_t location, uint8_t *charmap) {
    location &= 0x7; // we only have 8 locations 0-7
    // This command makes it so we write to the character ram
	_i2c_lcd_command(LCD_SET_CGR_ADR_CMD | (location << 3));
	for (int i=0; i<8; i++) {
		_i2c_lcd_send(&charmap[i], 1, MODE_4BIT, DATA_REGR);
	}
}

/*-------------------------------------------------------------------------- */
/* Internal lower level I2C code                                             */ 
/*---------------------------------------------------------------------------*/

static uint8_t _i2c_lcd_display_cmd () {
    return LCD_DISPLAY_CMD | _display | _cursor | _blink;
}

static uint8_t _i2c_lcd_entry_mode_cmd () {
    return LCD_ENTRY_MODE_CMD | _entry_mode | _shift_mode;
}

static void _i2c_lcd_set_address()
{
    // Critical section
    GLOBAL_INT_DISABLE();
    i2c_set_controller_status(I2C_CONTROLLER_DISABLE);
    // The programmer supports I2C memories with an address range between 0x00000 and 0x3FFFF (maximum size is 256KB)
    i2c_set_target_address(I2C_LCD_ADDRESS);  // Set Slave device address
    i2c_set_controller_status(I2C_CONTROLLER_ENABLE);
    while (i2c_is_master_busy());               // Wait until no master activity
    GLOBAL_INT_RESTORE();
}

void _i2c_lcd_command(const uint8_t byte) {
    uint8_t data[1] = { byte };
    _i2c_lcd_send(data, 1, MODE_4BIT, INST_REGR);
}

//@todo - explain this so I remember
void i2c_lcd_get_8bit_cmd(uint8_t byte, uint8_t buffer[3], uint8_t rs) {
    uint8_t b = (byte | rs) | _backlight;
    buffer[0] = b & 0xFB; // Enable Low
    buffer[1] = b | 0x04; // Enable High
    buffer[2] = b & 0xFB; // Enable Low
}

//@todo - explain this so I remember
void i2c_lcd_get_4bit_cmd(uint8_t byte, uint8_t buffer[6], uint8_t rs) {
    uint8_t hinib = (byte & 0xF0) | rs | _backlight;
    uint8_t lonib = (byte << 4 & 0xF0) | rs | _backlight;
    buffer[0] = hinib & 0xFB; // Enable Low
    buffer[1] = hinib | 0x04; // Enable High
    buffer[2] = hinib & 0xFB; // Enable Low
    buffer[3] = lonib & 0xFB; // Enable Low
    buffer[4] = lonib | 0x04; // Enable High
    buffer[5] = lonib & 0xFB; // Enable Low
}

/**
 * This method is a copy of the i2c i2c_master_transmit_buffer_sync with systick wait added
 * for the delays required by the HD44780
 */
 /**
 ******************************************************************************
 * Private function that uses the i2c driver (see i2c.h) to send data to the 
 * LCD. This method is a copy of the i2c i2c_master_transmit_buffer_sync with 
 * systick wait added for the delays required by the HD44780
 * 
 * @param[in] data  data pointer (typically a char array)
 * @param[in] len   length of [data] to send via i2c 
 * @note len is not checked here because it is interally set to 3 or 6
 ******************************************************************************
 */
uint16_t _i2c_send(const uint8_t *data, uint16_t len) {
    i2c_abort_t abrt_code;
    i2c_abort_t ret = I2C_ABORT_NONE;
    uint16_t bytes_written = 0;
    
    while (len--)
    {
        while (!i2c_is_tx_fifo_not_full());
        i2c_write_byte(data[bytes_written] | I2C_STOP);
        bytes_written++;
        
        // every 3rd command wait at least 37us
        if(bytes_written%3 == 0) {
            systick_wait(240);
        } else {
            // every command needs at least 450ns for the enable pin to be read
            systick_wait(200);
        }
        
        // Read tx abort source
        ret = i2c_get_abort_source();
        if (ret)
        {
            // Clear tx abort
            i2c_reset_int_tx_abort();
            break;
        }
    }
    if (!ret)
    {
        // Wait until TX fifo is empty
        while (!i2c_is_tx_fifo_empty());
        // Wait until no master activity
        while (i2c_is_master_busy());
    }
    if (abrt_code)
    {
        // break here for debugging i2c aborts
        abrt_code = ret;
    }
    return bytes_written;
}

/**
 ******************************************************************************
 * Private function that preps data in 4bit or 8bit mode for sending via i2c
 * 
 * @param[in] data  data pointer (typically a char array)
 * @param[in] len   length of [data] to print, if it is more than actual length 
 *                  of data, actual length is used.
 * @param[in] mode  MODE_4BIT or MODE_8BIT
 * @param[in] rs    INST_REGR or DATA_REGR for instruction or data registers
 ******************************************************************************
 */
uint16_t _i2c_lcd_send(const uint8_t *data, uint16_t len, uint8_t mode, 
    uint8_t rs) {
    // before the data is sent we set the i2c address for the LCD
    _i2c_lcd_set_address();
    
    uint16_t bytes_written = 0;
    uint16_t index = 0; 
    
    while (index < len)
    {
        // Mode will either be MODE_4BIT (0) or MODE_8BIT (1)
        // Data is sent in 8bit mode or 4bit mode.  8bit mode is used in the 
        // initialization phase until 4bit mode is ready (see i2c_lcd_init).
        // After that, 4bit mode is used for everything.
        if(mode == MODE_8BIT) {
            uint8_t cmd[3];
            i2c_lcd_get_8bit_cmd(data[index], cmd, rs);
            uint16_t num_bytes = _i2c_send(cmd, 3);
            bytes_written += num_bytes;
        } else {
            uint8_t cmd[6];
            i2c_lcd_get_4bit_cmd(data[index], cmd, rs);
            uint16_t num_bytes = _i2c_send(cmd, 6);
            bytes_written += num_bytes;
        }
        index++;
    }   
    return bytes_written;
}

/**
 ******************************************************************************
 * Private function used during initialization that sends a byte in 8bit mode 
 * to the instruction register and then waits for [us] microseconds.
 * 
 * @param[in] byte  lcd command to send
 * @param[in] us    microseconds to wait 
 ******************************************************************************
 */
void _i2c_send_and_wait_8bit(uint8_t byte, uint32_t us) {
    uint8_t data_8bit[1] = {byte};
    _i2c_lcd_send(data_8bit, 1, MODE_8BIT, INST_REGR);
    systick_wait(us);
}
