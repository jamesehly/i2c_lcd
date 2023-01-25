/**
 ****************************************************************************************
 *
 * @file user_periph_setup.h
 *
 * @brief Peripherals setup header file.
 *
 * Copyright (C) 2015-2020 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef _USER_PERIPH_SETUP_H_
#define _USER_PERIPH_SETUP_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "uart.h"
#include "gpio.h"
#include "i2c_eeprom.h"
#include "i2c_lcd.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/****************************************************************************************/
/* UART2 configuration to print messages                                                */
/****************************************************************************************/
#define UART                        UART2
// Define UART2 Tx Pad
#if defined (__DA14531__)
    #define UART2_TX_PORT           GPIO_PORT_0
    #define UART2_TX_PIN            GPIO_PIN_6
#else
    #define UART2_TX_PORT           GPIO_PORT_0
    #define UART2_TX_PIN            GPIO_PIN_4
#endif

// Define UART2 Settings
#define UART2_BAUDRATE              UART_BAUDRATE_115200
#define UART2_DATABITS              UART_DATABITS_8
#define UART2_PARITY                UART_PARITY_NONE
#define UART2_STOPBITS              UART_STOPBITS_1
#define UART2_AFCE                  UART_AFCE_DIS
#define UART2_FIFO                  UART_FIFO_EN
#define UART2_TX_FIFO_LEVEL         UART_TX_FIFO_LEVEL_0
#define UART2_RX_FIFO_LEVEL         UART_RX_FIFO_LEVEL_0

/****************************************************************************************/
/* I2C configuration                                                                    */
/****************************************************************************************/
// Define I2C Pins
#if defined (__DA14531__)
    #define I2C_SCL_PORT            GPIO_PORT_0
    #define I2C_SCL_PIN             GPIO_PIN_8
    #define I2C_SDA_PORT            GPIO_PORT_0
    #define I2C_SDA_PIN             GPIO_PIN_9
#else
    #define I2C_SCL_PORT            GPIO_PORT_2
    #define I2C_SCL_PIN             GPIO_PIN_3
    #define I2C_SDA_PORT            GPIO_PORT_2
    #define I2C_SDA_PIN             GPIO_PIN_1
#endif

#define I2C_LCD_ADDRESS         0x27
#define I2C_LCD_NUM_LINES       LCD_TWO_LINES
#define I2C_LCD_NUM_COLS        24
#define I2C_LCD_ENTRY_MODE      LCD_ENTRY_INC
#define I2C_LCD_SHIFT_MODE      LCD_SHIFT_OFF
#define I2C_LCD_FONT_MODE       LCD_FONT_5x8

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief   Initializes application's peripherals and pins
 ****************************************************************************************
 */
void periph_init(void);

/**
 ****************************************************************************************
 * @brief Set gpio port function mode
 ****************************************************************************************
 */
void set_pad_functions(void);

#endif // _USER_PERIPH_SETUP_H_
