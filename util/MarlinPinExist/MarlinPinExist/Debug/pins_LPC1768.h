/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Gen7 Alfons3 board pin assignments
 *
 * These Pins are assigned for the modified GEN7 Board from Alfons3.
 * Please review the pins and adjust them for your needs.
 */

#if !defined(__ARM_LPC1768__) 
  #error "Oops!  Make sure you have 'LPC1768' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME      "LPC1768"
#define _IPIN(p,q) ((p*32)+q)
#define _PIN_NUM(pq) (pq%32)
#define _PIN_PORT(pq) (pq/32)


#define	LED_PIN     _IPIN(4,28)  //   P4_28
#define     USBTX  P0_2
#define     USBRX  P0_3


//
// Limit Switches
//
#if ENABLED(DELTA)
  #define X_MAX_PIN       _IPIN(2,0) // P2_0  D2  = XMAX
  #define Y_MAX_PIN       _IPIN(0,1) // P0_1  D15 = YMAX
  #define Z_MAX_PIN       _IPIN(0,16)// P0_16 D19 = ZMAX
  #define Z_MIN_PIN       _IPIN(0,15)// P0_15 D18 = ZMIN
#else
  #define X_MIN_PIN       _IPIN(2,1) // P2_1  D3  = XMIN
  #define Y_MIN_PIN       _IPIN(0,0) // P0_0  D14 = YMIN
  #define Z_MIN_PIN       _IPIN(0,15)// P0_15 D18 = ZMIN
#endif

//
// Steppers
//
#define X_STEP_PIN	_IPIN(2,11)  //=  P2_11  A0 Arduino
#define X_DIR_PIN       _IPIN(2,12)  //   P2_12  A1
#define X_ENABLE_PIN    _IPIN(2,13)  //   P2_13  D38


#define Y_STEP_PIN      _IPIN(1,28)	// P1_28 A6
#define Y_DIR_PIN       _IPIN(1,29)	// P1_29 A7
#define Y_ENABLE_PIN    _IPIN(1,27)	// P1_27 A2

#define Z_STEP_PIN      _IPIN(0,17)	// P0_17 D46
#define Z_DIR_PIN       _IPIN(0,18)	// P0_18 D48
#define Z_ENABLE_PIN    _IPIN(0,21)	// P0_21 A8


#define E0_STEP_PIN     _IPIN(1,19)	// P1_19 D26

#ifdef DELTA
 #define E0_DIR_PIN      _IPIN(3,25)	// P3_25 D44
#else
 #define E0_DIR_PIN      _IPIN(1,20)	// P1_20 D28
#endif

#define E0_ENABLE_PIN   _IPIN(1,18)	// P1_18 D24

//#define E1_STEP_PIN     _IPIN(1,22)	// P1_22 ??

#ifdef DELTA
//#define E1_DIR_PIN      _IPIN(3,26)	// P3_26 D40
#else
//#define E1_DIR_PIN      _IPIN(1,23)	// P1_23 D34
#endif
//#define E1_ENABLE_PIN   _IPIN(1,21)	// P1_21 ??

//
// Temperature Sensors
//
// indexes of adc_pin_data adc_pin_map[] (0-7)
//0 = P0.23
//1 = P0.24
//2 = P0.25
//3 = P0.26
//4 = P1.30
//5 = P1.31
//6 = P0.3
//7 = P0.2

#define TEMP_0_PIN       3   // Analog Input
//#define TEMP_1_PIN       4   // Analog Input
#define TEMP_BED_PIN     5   // Analog Input (pin 34 bed)

//
// Heaters
//

#if ENABLED(IS_RAMPS_EFB)      // Hotend, Fan, Bed
#define HEATER_0_PIN     _IPIN(2,8)  // P2_8 RAMPS_D10_PIN
#define HEATER_BED_PIN   _IPIN(2,6)  // P2_6 (bed) = RAMPS_D8_PIN
#define FAN_PIN          _IPIN(2,7)  // P2_7 RAMPS_D9_PIN

#else

#define HEATER_0_PIN     _IPIN(2,8)  // P2_8  RAMPS_D10_PIN
#define HEATER_BED_PIN   _IPIN(2,6)  // P2_6  (bed) = RAMPS_D8_PIN

#endif

//
// Misc. Functions
//
//#define SDSS            31  // SCL pin of I2C header || CS Pin for SD Card support
//#define PS_ON_PIN       19

// A pin for debugging
#define DEBUG_PIN       -1

//
// LCD / Controller
//
//LCD_CLASS lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7); //RS,Enable,D4,D5,D6,D7

#define BEEPER_PIN      _IPIN(0,5)  // D37

// 4bit LCD Support
#define LCD_PINS_RS     _IPIN(0,10) // D16
#define LCD_PINS_ENABLE _IPIN(0,11) // D17
#define LCD_PINS_D4     _IPIN(0,27) // D23
#define LCD_PINS_D5     _IPIN(0,29) // D25
#define LCD_PINS_D6     _IPIN(0,30) // D27
#define LCD_PINS_D7     _IPIN(0,4)  // D29

// Buttons are directly attached
#define BTN_EN1         _IPIN(1,24) // D31
#define BTN_EN2         _IPIN(1,25) // D33
#define BTN_ENC         _IPIN(1,26) // D35

// RS485 pins
//#define TX_ENABLE_PIN   12
//#define RX_ENABLE_PIN   13
