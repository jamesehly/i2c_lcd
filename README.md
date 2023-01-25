# I2C LCD Driver for DA1453x

This LCD driver is for the [DA145xx family](https://www.renesas.com/eu/en/products/interface-connectivity/wireless-communications/bluetooth-low-energy/da14531-smartbond-ultra-low-power-bluetooth-51-system-chip)
and any PCF8574 I2C chip driving a HD44780U compatible LCD module.

This driver is based largly off the work done on the [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
library for Arduino. The API is intended to be similar but not all functionality is included yet.

## Running the Example

The example included in this repo initializes the LCD module and steps through the driver API.

To run this example, the user needs:

- DA14531/DA14585/DA14586 hardware
- A LCD module with an I2C backpack
- Keil µvision
- Dialog SDK 6.0.16.1144

The user needs to clone this repository next to the i2c_eeprom example in located at

```
~\6.0.16.1144\projects\target_apps\peripheral_examples\i2c
```

After cloning you should have 2 folders located in the i2c folder: i2c_eeprom and i2c_lcd. Like this:

![File system location for i2c_lcd driver](https://raw.githubusercontent.com/jamesehly/i2c_lcd/assets/file-system-location.png)

If the repo is placed in this location, all of the files should be linked correctly.

Open the example project with Keil µvision located at:

~\6.0.16.1144\projects\target_apps\peripheral_examples\i2c\i2c_lcd\example\Keil_5\i2c_lcd.uvprojx

Running the example should produce output like this:

https://www.youtube.com/watch?v=h-n-F97LMy8

## Hooking up the LCD Module

In order to hook up the LCD module (which runs at 5 volts) to the DA1453x
(which runs at 3.3v) you will need a [logic level converter circuit](https://a.co/d/b4CZBB3).  
Determine and set the SCL and SDA pins on the DA1453x in the user_periph_setup.h file.
Ine the example these are pins 8 and 9:

```
#define I2C_SCL_PORT            GPIO_PORT_0
#define I2C_SCL_PIN             GPIO_PIN_8
#define I2C_SDA_PORT            GPIO_PORT_0
#define I2C_SDA_PIN             GPIO_PIN_9
```

These pins should be connected to the low level side of the logic level converter.
The high side of the logic level converter should be hooked up the SCL and SDA pins
on the LCD I2C backpack module.

Obviously you will need a 5v and a 3.3v supply for the LCD module and the DA1453x.  
Conveniently you can get this from a single and inexpensive [ST-LINK v2 emulator](https://a.co/d/3ZGsp5P)
on Amazon.

Set the LCD config options according to your LCD setup:

```
#define I2C_LCD_ADDRESS         0x27
#define I2C_LCD_NUM_LINES       LCD_TWO_LINES
#define I2C_LCD_NUM_COLS        24
#define I2C_LCD_ENTRY_MODE      LCD_ENTRY_INC
#define I2C_LCD_SHIFT_MODE      LCD_SHIFT_OFF
#define I2C_LCD_FONT_MODE       LCD_FONT_5x8
```

## Testing

So far I have tested this driver with the DA14531 module and a 2402-1 Series Character LCD Module.

## @TODO...

- Test, test, test. I need people to test out the library to harden it and find bugs. I have no
  idea if it works with chips other than the DA14531 and other sizes of LCD Screens besides 24x2
- Implement 4 line screens
- Complete all API functions included in LiquidCrystal_I2C
