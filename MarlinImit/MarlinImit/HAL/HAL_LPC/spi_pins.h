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

#ifndef SPI_PINS_LPC1768_H
#define SPI_PINS_LPC1768_H

//#define LPC_SOFTWARE_SPI

/** onboard SD card */
#define SCK_PIN         _IPIN(0,7)  // P0_7 = D52 SSP1
#define MISO_PIN        _IPIN(0,8)  // P0_8 = D50
#define MOSI_PIN        _IPIN(0,9)  // P0_9 = D51 
#define SS_PIN          _IPIN(0,6)  // P0_6 = D53

/** external */
//#define SCK_PIN           P0_15 //SSP0
//#define MISO_PIN          P0_17
//#define MOSI_PIN          P0_18
//#define SS_PIN            P1_23

#define SDSS              SS_PIN
//#define USE_SSP0

#if (defined(TARGET_LPC1768) && !(defined(LPC_SOFTWARE_SPI)))   // signal LCDs that they need to use the hardware SPI
  #define SHARED_SPI
#endif

#endif /* SPI_PINS_LPC1768_H */
