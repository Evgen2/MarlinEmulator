/* HAL_timers_LPC1768.h */

#ifndef _HAL_TIMERS_LPC1768_H
#define _HAL_TIMERS_LPC1768_H


typedef uint32_t hal_timer_t;

#define STEP_TIMER_NUM 2  // index of timer to use for stepper
#define TEMP_TIMER_NUM 1  // index of timer to use for temperature

#define HAL_TIMER_RATE         ((SystemCoreClock) / 4)  // frequency of timers peripherals
#define STEPPER_TIMER_PRESCALE 1.0              // prescaler for setting stepper frequency
//#define HAL_STEPPER_TIMER_RATE HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_STEPPER_TIMER_RATE 25000000   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TICKS_PER_US       ((HAL_STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per us

#define HAL_TEMP_TIMER_RATE    1000000
#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define STEP_TIMER_MIN_INTERVAL   8 // minimum time in µs between stepper interrupts

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt (STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt (STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt (TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt (TEMP_TIMER_NUM)

#define HAL_ENABLE_ISRs() do { if (thermalManager.in_temp_isr)DISABLE_TEMPERATURE_INTERRUPT(); else ENABLE_TEMPERATURE_INTERRUPT(); ENABLE_STEPPER_DRIVER_INTERRUPT(); } while(0)

#define HAL_STEP_TIMER_ISR  extern "C" void TIMER2_IRQHandler(void)
#define HAL_TEMP_TIMER_ISR  extern "C" void TIMER1_IRQHandler(void)




// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------
extern "C" void setupTimers(void);
void HAL_timer_start (uint8_t timer_num, uint32_t frequency);

void HAL_timer_set_count (uint8_t timer_num, hal_timer_t count);
hal_timer_t HAL_timer_get_count (uint8_t timer_num);
void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks);
hal_timer_t HAL_timer_get_current_count(uint8_t timer_num);

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare);
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num);

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);
void HAL_timer_isr_prologue(const uint8_t timer_num);

#endif //_HAL_TIMERS_LPC1768_H
