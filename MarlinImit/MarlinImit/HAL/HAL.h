/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
 * Description: HAL wrapper
 *
 * Supports platforms :
 *    ARDUINO_ARCH_SAM : For Arduino Due and other boards based on Atmel SAM3X8E
 *    __AVR__ : For all Atmel AVR boards
 */

#ifndef _HAL_H
#define _HAL_H

#include "SPI.h"

#ifdef __AVR__
  #include "HAL_AVR/HAL_AVR.h"
#elif defined(ARDUINO_ARCH_SAM)
  #define CPU_32_BIT
  #include "HAL_DUE/HAL_Due.h"
  #include "math_32bit.h"
#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
  #define CPU_32_BIT
  #include "HAL_TEENSY35_36/HAL_Teensy.h"
  #include "math_32bit.h"
#elif defined(TARGET_LPC1768)
  #define CPU_32_BIT
  #include "math_32bit.h"
  #include "HAL_LPC/HAL_LPC1768.h"
#elif defined(__STM32F1__)
  #define CPU_32_BIT
  #include "math_32bit.h"
  #include "HAL_STM32F1/HAL_Stm32f1.h"
#else
  #error "Unsupported Platform!"
#endif

#endif // _HAL_H
