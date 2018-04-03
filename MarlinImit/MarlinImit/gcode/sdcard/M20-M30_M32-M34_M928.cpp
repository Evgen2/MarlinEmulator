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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../module/printcounter.h"
#include "../../module/stepper.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../feature/pause.h"
  #include "../queue.h"
#endif

/**
 * M20: List SD card to serial output
 */
void GcodeSuite::M20() {
  SERIAL_PROTOCOLLNPGM(MSG_BEGIN_FILE_LIST);
  card.ls();
  SERIAL_PROTOCOLLNPGM(MSG_END_FILE_LIST);
}

/**
 * M21: Init SD Card
 */
void GcodeSuite::M21() { card.initsd(); }

/**
 * M22: Release SD Card
 */
void GcodeSuite::M22() { card.release(); }

/**
 * M23: Open a file
 */
void GcodeSuite::M23() {
  // Simplify3D includes the size, so zero out all spaces (#7227)
  for (char *fn = parser.string_arg; *fn; ++fn) if (*fn == ' ') *fn = '\0';
  card.openFile(parser.string_arg, true);
}

/**
 * M24: Start or Resume SD Print
 */
void GcodeSuite::M24() {
  #if ENABLED(PARK_HEAD_ON_PAUSE)
    resume_print();
  #endif

  card.startFileprint();
  print_job_timer.start();
}

/**
 * M25: Pause SD Print
 */
void GcodeSuite::M25() {
  card.pauseSDPrint();
  print_job_timer.pause();

  #if ENABLED(PARK_HEAD_ON_PAUSE)
    enqueue_and_echo_commands_P(PSTR("M125")); // Must be enqueued with pauseSDPrint set to be last in the buffer
  #endif
}

/**
 * M26: Set SD Card file index
 */
void GcodeSuite::M26() {
  if (card.cardOK && parser.seenval('S'))
    card.setIndex(parser.value_long());
}

/**
 * M27: Get SD Card status
 */
void GcodeSuite::M27() { card.getStatus(); }

/**
 * M28: Start SD Write
 */
void GcodeSuite::M28() { card.openFile(parser.string_arg, false); }

/**
 * M29: Stop SD Write
 * Processed in write to file routine
 */
void GcodeSuite::M29() {
  // card.saving = false;
}

/**
 * M30 <filename>: Delete SD Card file
 */
void GcodeSuite::M30() {
  if (card.cardOK) {
    card.closefile();
    card.removeFile(parser.string_arg);
  }
}

/**
 * M32: Select file and start SD Print
 */
void GcodeSuite::M32() {
  if (IS_SD_PRINTING)
    stepper.synchronize();

  char* namestartpos = parser.string_arg;
  const bool call_procedure = parser.boolval('P');

  if (card.cardOK) {
    card.openFile(namestartpos, true, call_procedure);

    if (parser.seenval('S'))
      card.setIndex(parser.value_long());

    card.startFileprint();

    // Procedure calls count as normal print time.
    if (!call_procedure) print_job_timer.start();
  }
}

#if ENABLED(LONG_FILENAME_HOST_SUPPORT)

  /**
   * M33: Get the long full path of a file or folder
   *
   * Parameters:
   *   <dospath> Case-insensitive DOS-style path to a file or folder
   *
   * Example:
   *   M33 miscel~1/armchair/armcha~1.gco
   *
   * Output:
   *   /Miscellaneous/Armchair/Armchair.gcode
   */
  void GcodeSuite::M33() {
    card.printLongPath(parser.string_arg);
  }

#endif // LONG_FILENAME_HOST_SUPPORT

#if ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_GCODE)

  /**
   * M34: Set SD Card Sorting Options
   */
  void GcodeSuite::M34() {
    if (parser.seen('S')) card.setSortOn(parser.value_bool());
    if (parser.seenval('F')) {
      const int v = parser.value_long();
      card.setSortFolders(v < 0 ? -1 : v > 0 ? 1 : 0);
    }
    //if (parser.seen('R')) card.setSortReverse(parser.value_bool());
  }

#endif // SDCARD_SORT_ALPHA && SDSORT_GCODE

/**
 * M928: Start SD Write
 */
void GcodeSuite::M928() {
  card.openLogFile(parser.string_arg);
}

#endif // SDSUPPORT
