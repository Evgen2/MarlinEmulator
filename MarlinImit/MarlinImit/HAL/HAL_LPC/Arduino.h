/* Arduino.h */
#ifndef Arduino_h
#define Arduino_h

#include <stdarg.h>
#include "binary.h"


// Program Memory
#define pgm_read_ptr(address_short) (*(address_short))
#define pgm_read_byte_near(address_short) (*address_short)
#define pgm_read_byte(address_short) pgm_read_byte_near(address_short)
#define pgm_read_float_near(address_short) (*address_short)
#define pgm_read_float(address_short) pgm_read_float_near(address_short)
#define pgm_read_word_near(address_short) (*address_short)
#define pgm_read_word(address_short) pgm_read_word_near(address_short)
#define pgm_read_dword_near(address_short) (*address_short)
#define pgm_read_dword(address_short) pgm_read_dword_near(address_short)

#define memcpy_P memcpy
#define sprintf_P sprintf
#define strstr_P strstr
#define strncpy_P strncpy
#define vsnprintf_P vsnprintf
#define strcpy_P strcpy
#define snprintf_P snprintf


#endif //Arduino_h
