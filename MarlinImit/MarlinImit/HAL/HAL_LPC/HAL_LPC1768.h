/* HAL_LPC1768.h */

#ifndef _HAL_LPC1768_H
#define _HAL_LPC1768_H


#include "arduino.h"
#include "PinNames.h"
#include "fastio_LPC1768.h"
#include "HAL_timers_LPC1768.h"
#include "LPC1768Serial.h"
#include "watchdog_LPC1768.h"

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ( -1)
#endif


extern void pinMode( uint32_t dwPin, uint32_t dwMode ) ;
//extern void pinMode( PinName dwPin, uint32_t dwMode ); 

extern void digitalWrite( uint32_t dwPin, uint32_t dwVal ) ;
extern void analogWrite( uint32_t ulPin, uint32_t ulValue ) ;
extern int digitalRead( uint32_t ulPin ) ;

extern void _delay_ms(int delay);
extern void delay(int delay);


// Disable interrupts
void cli(void);
//	 __disable_irq();

// Enable interrupts
void sei(void);
//	 __enable_irq();

#define CRITICAL_SECTION_START	 __disable_irq();
#define CRITICAL_SECTION_END     __enable_irq();

/** clear reset reason */
void HAL_clear_reset_source (void);

/** reset reason */
uint8_t HAL_get_reset_source (void);


int freeMemory(void);


struct pin_data { uint8_t port, pin; };
struct adc_pin_data { uint8_t port, pin, adc; };

// ADC
#define NUM_ANALOG_INPUTS 8

#define HAL_ANALOG_SELECT(pin) HAL_adc_enable_channel(pin)
#define HAL_START_ADC(pin)     HAL_adc_start_conversion(pin)
#define HAL_READ_ADC           HAL_adc_get_result()


void HAL_adc_init(void);
void HAL_adc_enable_channel(int pin);
void HAL_adc_start_conversion (uint8_t adc_pin);
uint16_t HAL_adc_get_result(void);

/** result of last ADC conversion */
extern uint16_t HAL_adc_result;

/*
 * \brief Writes an analog value (PWM wave) to a pin.
 *
 * \param ulPin
 * \param ulValue
 */
extern void analogWrite( uint32_t ulPin, uint32_t ulValue ) ;
extern uint32_t analogRead( uint32_t ulPin ) ;

extern void digitalWrite( uint32_t dwPin, uint32_t dwVal ) ;
extern int digitalRead( uint32_t ulPin ) ;

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)
#define NUM_SERIAL 1

#endif // _HAL_LPC1768_H
