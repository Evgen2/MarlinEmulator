/* HAL_timers_LPC1768.cpp */

#include "../../config_0.h"
#ifdef __ARM_LPC1768__

#include "mbed.h"
#include "../HAL.h"
#include "HAL_timers_LPC1768.h"
#include "../../inc/MarlinConfig.h"

extern "C" void setupTimers(void);
extern void HeaterOn(int onOff);

volatile uint32_t _millis=0; //  millisecond global counter

//Timer timer0; //TIMER_3

/****************************************/
unsigned long millis(void)
{
	return _millis;
}

#if defined(USE_TMP03)

volatile uint32_t _last_Heater_control_millis=0; 
volatile int HeaterOnOff = 0;
void  SetCheckHeater(int onoff)
{
	 if(onoff)
	 {	 _last_Heater_control_millis = _millis;
	 }
	 HeaterOnOff = onoff;
}

void  checkHeater(void)
{
	 if(HeaterOnOff && ( (_millis - _last_Heater_control_millis) > 60000*3) ) // 3 минуты
	 {  HeaterOn(0);
	 }
}
	
#endif

extern "C" {

	//extern void disk_timerproc(void);
void SysTick_Handler(void) {
  ++_millis;
#if defined(USE_TMP03)
   checkHeater();
#endif
//  disk_timerproc(); /* Disk timer process */
}
}

/****************************************/

// Keil: Timer 0 is used for TCP retransmission control, init at void TCPLowLevelInit(void)
// #define STEP_TIMER_NUM 2  // index of timer to use for stepper
// #define TEMP_TIMER_NUM 1  // index of timer to use for temperature
//Timer3 is used for TMP03
void setupTimers(void)
{   //timer0.start();
  LPC_SC->PCONP |= (0x1 << 22);  // power on timer2
  LPC_TIM2->PR = ((HAL_TIMER_RATE / HAL_STEPPER_TIMER_RATE) - 1); // Use prescaler to set frequency if needed

  LPC_SC->PCONP |= (0x1 << 0x2);  // power on timer1
  LPC_TIM1->PR = ((HAL_TIMER_RATE / 1000000) - 1);

  LPC_SC->PCONP |= (0x1 << 23);  // power on timer3
  LPC_TIM3->TCR=1;
	
}

void HAL_timer_start (uint8_t timer_num, uint32_t frequency)
{
  switch(timer_num) {
  case STEP_TIMER_NUM:
    LPC_TIM2->MCR = 3;              // Match on MR0, reset on MR0
    LPC_TIM2->MR0 = (uint32_t)(HAL_STEPPER_TIMER_RATE / frequency); // Match value (period) to set frequency
	LPC_TIM2->TC = 0;
    LPC_TIM2->TCR = (1 << 0);       // enable
    break;
  case TEMP_TIMER_NUM:
    LPC_TIM1->MCR = 3;
    LPC_TIM1->MR0 = (uint32_t)(HAL_TEMP_TIMER_RATE / frequency);
    LPC_TIM1->TCR = (1 << 0);
	LPC_TIM1->TC = 0;
    break;
  default:
    return;
  }	
}

void HAL_timer_enable_interrupt (uint8_t timer_num) {
  switch(timer_num) {
  case STEP_TIMER_NUM:
    NVIC_EnableIRQ(TIMER2_IRQn);     // Enable interrupt handler
    NVIC_SetPriority(TIMER2_IRQn, NVIC_EncodePriority(0, 1, 0));
    break;
  case TEMP_TIMER_NUM:
    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_SetPriority(TIMER1_IRQn, NVIC_EncodePriority(0, 2, 0));
    break;
  }
}

void HAL_timer_disable_interrupt (uint8_t timer_num) {
  switch(timer_num) {
  case STEP_TIMER_NUM:
    NVIC_DisableIRQ(TIMER2_IRQn);     // disable interrupt handler
    break;
  case TEMP_TIMER_NUM:
    NVIC_DisableIRQ(TIMER1_IRQn);
    break;
  }	
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
#ifndef WIN32	
  switch (timer_num) {
    case STEP_TIMER_NUM: return NVIC_GetActive(TIMER2_IRQn);
    case TEMP_TIMER_NUM: return NVIC_GetActive(TIMER1_IRQn);
  }
#endif
  return false;
}

void HAL_timer_isr_prologue (uint8_t timer_num) {
  switch(timer_num) {
  case STEP_TIMER_NUM:
    LPC_TIM2->IR |= 1; //Clear the Interrupt
    break;
  case TEMP_TIMER_NUM:
    LPC_TIM1->IR |= 1;
    break;
  }
}

	
void HAL_timer_set_count (uint8_t timer_num, hal_timer_t count) 
{
  switch(timer_num) {
  case STEP_TIMER_NUM:
    LPC_TIM2->MR0 = count;
    break;
  case TEMP_TIMER_NUM:
    LPC_TIM1->MR0 = count;
    break;
  default:
    return;
  }
}

hal_timer_t HAL_timer_get_count (uint8_t timer_num) {
  switch(timer_num) {
  case STEP_TIMER_NUM:
    return LPC_TIM2->MR0;
  case TEMP_TIMER_NUM:
    return LPC_TIM1->MR0;
  default:
    return 0;
  }
}

hal_timer_t HAL_timer_get_current_count(uint8_t timer_num) {
  switch(timer_num) {
  case STEP_TIMER_NUM:
    return LPC_TIM2->TC;
  case TEMP_TIMER_NUM:
    return LPC_TIM1->TC;
  default:
    return 0;
  }
}

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
	switch (timer_num) {
	case STEP_TIMER_NUM:
		LPC_TIM2->MR0 = compare;
		if (LPC_TIM2->TC > compare)
			LPC_TIM2->TC = compare - 5; // generate an immediate stepper ISR
		break;
	case TEMP_TIMER_NUM:
		LPC_TIM1->MR0 = compare;
		if (LPC_TIM1->TC > compare)
			LPC_TIM1->TC = compare - 5; // make sure we don't have one extra long period
		break;
	}
}

hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
	switch (timer_num) {
	case STEP_TIMER_NUM: return LPC_TIM2->MR0;
	case TEMP_TIMER_NUM: return LPC_TIM1->MR0;
	}
	return 0;
}

void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks) {
	const hal_timer_t mincmp = HAL_timer_get_count(timer_num) + interval_ticks;
	if (HAL_timer_get_compare(timer_num) < mincmp) HAL_timer_set_compare(timer_num, mincmp);
}

#endif //__ARM_LPC1768__
