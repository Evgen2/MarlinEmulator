/* config_0.h */

#ifndef __CONFIG_0__
#define __CONFIG_0__
#include <stdlib.h>


//emulator
	#if defined (WIN32)
//real chip
//	#if defined (__TARGET_CPU_CORTEX_M3)

		#define __ARM_LPC1768__
//		#pragma anon_unions
//			#if defined (__STDC__)
//				#define inline  __inline
//				#define bool  int
//			#endif
#ifndef TARGET_LPC1768
#define TARGET_LPC1768
#endif

#define NOTUSEASSERT

#define PROGMEM

#define sq(x) ((x)*(x))

//todo
//??#define F_CPU  100000000

unsigned long millis(void);
#define PSTR(x) x
#define PGM_P char *
#define sprintf_P sprintf
#define strncpy_P strncpy
#define strstr_P strstr

#if defined(WIN32)
	#define constexpr const
	#define strcasecmp _stricmp
#else
	#define constexpr
#endif

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


/* Minimum and maximum macros */
 
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))


//  pinMode(pin_number, OUTPUT);
#define INPUT  0
#define INPUT_PULLUP 1
#define OUTPUT 2

	#endif

#define PASS      0
#define FAIL      1
#define LOW       0
#define HIGH      1

#define false 0
#define true  1

typedef int pin_t;

//for use of lround etc
#define __USE_C99_MATH

extern "C" void setup(void);
extern "C" void loop(void);

extern void todo(const char *str);

extern "C" void net_stuff(void);

//#define M114_DETAIL
#endif  //__CONFIG_0__
