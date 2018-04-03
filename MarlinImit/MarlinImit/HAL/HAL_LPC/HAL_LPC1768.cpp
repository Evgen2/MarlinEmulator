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
 * Description: HAL for LPC1768
 *
 * For ARDUINO_ARCH_SAM
 */

#include "../../config_0.h"
#ifdef __ARM_LPC1768__
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "mbed.h"
#include "wait_api.h"
#include "../HAL.h"
#include "../../inc/MarlinConfig.h"

#include "../../mybuffer.hpp"

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------
void HAL_TMP03_init(void);

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

uint16_t HAL_adc_result;

myBuffer2 debugBuff;
unsigned int debugBuff2[2048];

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

#include "DigitalInOut.h"

DigitalInOut pio[32*5];

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------
void err2(const char *str, const char *str1);
void err3(const char *str, const char *str1, int par);
int median_of_3(int a, int b, int c);
uint16_t HAL_adc_get_result(void);
bool HAL_adc_finished(void);

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------
extern "C" void test00(void);

// disable interrupts
void cli(void)
{
//todo
	 __disable_irq();
//    todo(__FUNCTION__);

}

// enable interrupts
void sei(void)
{
//todo
	__enable_irq();
//    todo(__FUNCTION__);
}


void _delay_ms(int delay_ms)
{
//todo
//	delay (delay_ms);
//    todo(__FUNCTION__);
	wait_ms(delay_ms);
}

void delay(int delay)
{
//todo
//	delay (delay_ms);
	wait_ms(delay);
//    todo(__FUNCTION__);
}

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

// return free memory between end of heap (or end bss) and whatever is current
int freeMemory()
{
//todo
//  int free_memory, heap_end = (int)_sbrk(0);
//  return (int)&free_memory - (heap_end ? heap_end : (int)&_ebss);
    todo(__FUNCTION__);
  return 0;
}

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------
#define ADC_DONE      0x80000000
#define ADC_OVERRUN   0x40000000
float ADC_values[8]={0,0,0,0, 0,0,0,0};
uint32_t ADC_count[8]={0,0,0,0, 0,0,0,0};

void HAL_adc_init(void) 
{ int i; 
  LPC_SC->PCONP |= (1 << 12);      // Enable CLOCK for internal ADC controller
  LPC_SC->PCLKSEL0 &= ~(0x3 << 24);
  LPC_SC->PCLKSEL0 |= (0x0 << 24); // 0: 25MHz, 1: 100MHz, 2: 50MHz, 3: 12.5MHZ to ADC clock divider
  LPC_ADC->ADCR = (0 << 0)         // SEL: 0 = no channels selected
//                | (0xFF << 8)      // select slowest clock for A/D conversion 150 - 190 uS for a complete conversion
                | (0x10 << 8)      // select slowest clock for A/D conversion 150 - 190 uS for a complete conversion
                | (0 << 16)        // BURST: 0 = software control
                | (0 << 17)        // CLKS: not applicable
                | (1 << 21)        // PDN: 1 = operational
                | (0 << 24)        // START: 0 = no start
                | (0 << 27);       // EDGE: not applicable
	
	LPC_ADC->ADINTEN=0;
	
  debugBuff.Init(debugBuff2, sizeof(debugBuff2), 2);	
	_set_pin_output(_IPIN(0,23));  _write(_IPIN(0,23), 1);
	_set_pin_output(_IPIN(0,24));  _write(_IPIN(0,24), 1);
	_set_pin_output(_IPIN(0,25));  _write(_IPIN(0,25), 1);
	_set_pin_output(_IPIN(0,26));  _write(_IPIN(0,26), 1);
	_set_pin_output(_IPIN(1,30));  _write(_IPIN(1,30), 1);
	_set_pin_output(_IPIN(1,31));  _write(_IPIN(1,31), 1);
//_set_pin_output(_IPIN(0,3));  _write(_IPIN(0,3), 1);
//_set_pin_output(_IPIN(0,2));  _write(_IPIN(0,2), 1);
   for(i=0; i<8; i++)
			ADC_count[i] = 0;
			
#if MOTHERBOARD == BOARD_RAMPS_14_LPC1768_LS_MINI
   HAL_TMP03_init();
  #if PIN_EXISTS(ZATVOR)
    OUT_WRITE(ZATVOR_PIN, LOW) ; // turn it off
  #endif
  #if PIN_EXISTS(LED1)
    OUT_WRITE(LED1_PIN, LOW); // turn it on
  #endif
  #if PIN_EXISTS(RED_LASER)
    OUT_WRITE(RED_LASER_PIN, LOW) ; // turn it off
  #endif
  #if PIN_EXISTS(HEATER)
    OUT_WRITE(HEATER_PIN, LOW) ; // turn it off
  #endif

#endif

}

const adc_pin_data adc_pin_map[] = {
  {0, 23, 0},         //A0 (T0) - D67 - TEMP_0_PIN
  {0, 24, 1},         //A1 (T1) - D68 - TEMP_BED_PIN
  {0, 25, 2},         //A2 (T2) - D69 - TEMP_1_PIN
  {0, 26, 3},         //A3 - D63  P0.26 T_E0
  {1, 30, 4},         //A4 - D37 - BUZZER_PIN
  {1, 31, 5},         //A5 - D49 - SD_DETECT_PIN  P1.31 - T_Bed
  {0,  3, 6},         //A6 - D0  - RXD0
  {0,  2, 7}          //A7 - D1  - TXD0
};

void HAL_adc_enable_channel(int pin) {
  if (pin < 0 || pin >= NUM_ANALOG_INPUTS)  {
		err3(__FUNCTION__, " INVALID ANALOG PORT:",  pin);
		return;
  }

  int8_t pin_port = adc_pin_map[pin].port;
  int8_t pin_port_pin = adc_pin_map[pin].pin;
  int8_t pinsel_start_bit = pin_port_pin > 15 ? 2 * (pin_port_pin - 16) : 2 * pin_port_pin;
  uint8_t pin_sel_register = (pin_port == 0 && pin_port_pin <= 15) ? 0 :
                             (pin_port == 0)                       ? 1 :
                              pin_port == 1                        ? 3 : 10;

  switch (pin_sel_register) {
    case 1 :
      LPC_PINCON->PINSEL1 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL1 |=  (0x1 << pinsel_start_bit);
      LPC_PINCON->PINMODE1 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINMODE1 |=  (0x2 << pinsel_start_bit);
		
      break;
    case 3 :
      LPC_PINCON->PINSEL3 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL3 |=  (0x3 << pinsel_start_bit);
      LPC_PINCON->PINMODE3 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINMODE3 |=  (0x2 << pinsel_start_bit);
      break;
    case 0 :
      LPC_PINCON->PINSEL0 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINSEL0 |=  (0x2 << pinsel_start_bit);
      LPC_PINCON->PINMODE0 &= ~(0x3 << pinsel_start_bit);
      LPC_PINCON->PINMODE0 |=  (0x2 << pinsel_start_bit);
      break;
  };
}

void HAL_adc_start_conversion (uint8_t adc_pin)
{
  if(  (adc_pin >= NUM_ANALOG_INPUTS) || (adc_pin_map[adc_pin].port == 0xFF) ) {
		err3(__FUNCTION__, "HAL: HAL_adc_start_conversion: no pinmap for %d", adc_pin);
    return;
  }
  LPC_ADC->ADCR &= ~0xFF;                                // Reset
  LPC_ADC->ADCR |= ( 0x01 << adc_pin_map[adc_pin].adc ); // Select Channel
  LPC_ADC->ADCR |= ( 0x01 << 24 );                       // start conversion
}

bool HAL_adc_finished(void) {
  return LPC_ADC->ADGDR & ADC_DONE;
}

uint32_t t_n=0, t_nb=0, t_h0=0, t_h1=0;

static uint16_t mid1[8];
static uint16_t mid2[8];

uint16_t HAL_adc_get_result(void) {
	int id, chan;
//  uint32_t data = LPC_ADC->ADGDR;
	uint32_t sts, v0, v00;
	float v, res;
/***********************/
//	while(!(LPC_ADC->ADGDR & ADC_DONE)) ;
/***********************/
	chan = LPC_ADC->ADCR & 0xff;
	id = 0;
	if(chan & 0x02) id = 1;
	else if(chan & 0x04) id = 2;
	else if(chan & 0x08) id = 3;
	else if(chan & 0x10) id = 4;
	else if(chan & 0x20) id = 5;
	else if(chan & 0x40) id = 6;
	else if(chan & 0x80) id = 7;

//tst
if(chan != 1<<id)
  t_h0++;	
 	LPC_ADC->ADCR &= ~(1 << 24); //stop conversion
if(LPC_ADC->ADCR & (1 << 24))
  t_h1++;	
	
sts = LPC_ADC->ADSTAT;
#ifndef  WIN32

	if(!(sts & chan) || ((sts>>8) & chan) ) // not done  or overrun
	{
		 if(ADC_count[id] < 16)
		 {  if(ADC_count[id] == 0) 
			          return 0;
			  res = ADC_values[id] / ADC_count[id];
		 } else {
		    res = ADC_values[id]; 
		 }
		 return (uint16_t)res;
	}
#endif // ! WIN32

	v00 =  (( LPC_ADC->DR[id] >> 4) & 0xfff); //12bit
	
	t_n++;
		
	if(ADC_count[id] < 16)
	{  
		ADC_values[id] += (float)v00; 
		ADC_count[id]++;
		if(ADC_count[id] == 16)
		{  ADC_values[id] /=  16.;
		   mid1[id] = mid2[id] = (uint16_t) ADC_values[id];
		}
		 return v00;
	} else {
	if(v00 == 0)
	{  t_nb++;
  return (uint16_t) ADC_values[id]; //(res>>2); //10bit		 
	}
		v0 = median_of_3(mid1[id],mid2[id],v00);
		mid1[id] = mid2[id];
		mid2[id] = v00;
		res = ( (float)v0 + ADC_values[id] *255.)/ 256.;
#if 0	
	if(id == 5)
	{  uint32_t data[2];
extern volatile uint32_t _millis; //  millisecond global counter
		 data[0] = _millis;
		 data[1] = v0; 
	  debugBuff.Add(data);
	}
#endif //0	
		ADC_values[id] = res;
  }
  return (uint16_t)res; //(res>>2); //10bit
}

/*
 * \brief Writes an analog value (PWM wave) to a pin.
 *
 * \param ulPin
 * \param ulValue
 */
void analogWrite( uint32_t ulPin, uint32_t ulValue ) 
{
//todo
    todo(__FUNCTION__);
 ;
}

/*
 * \brief Reads the value from the specified analog pin.
 *
 * \param ulPin
 *
 * \return Read value from selected pin, if no error.
 */
uint32_t analogRead( uint32_t ulPin ) 
{
//todo
 ;
    todo(__FUNCTION__);
	return 0;
}


/**
 * \brief Write a HIGH or a LOW value to a digital pin.
 *
 * If the pin has been configured as an OUTPUT with pinMode(), its voltage will be set to the
 * corresponding value: 5V (or 3.3V on 3.3V boards) for HIGH, 0V (ground) for LOW.
 *
 * If the pin is configured as an INPUT, writing a HIGH value with digitalWrite() will enable an internal
 * 20K pullup resistor (see the tutorial on digital pins). Writing LOW will disable the pullup. The pullup
 * resistor is enough to light an LED dimly, so if LEDs appear to work, but very dimly, this is a likely
 * cause. The remedy is to set the pin to an output with the pinMode() function.
 *
 * \note Digital pin PIN_LED is harder to use as a digital input than the other digital pins because it has an LED
 * and resistor attached to it that's soldered to the board on most boards. If you enable its internal 20k pull-up
 * resistor, it will hang at around 1.7 V instead of the expected 5V because the onboard LED and series resistor
 * pull the voltage level down, meaning it always returns LOW. If you must use pin PIN_LED as a digital input, use an
 * external pull down resistor.
 *
 * \param dwPin the pin number
 * \param dwVal HIGH or LOW
 */
void digitalWrite( uint32_t dwPin, uint32_t dwVal )
{
     _write(dwPin, dwVal);

	//todo
 //   todo(__FUNCTION__);
 ;
}

/**
 * \brief Reads the value from a specified digital pin, either HIGH or LOW.
 *
 * \param ulPin The number of the digital pin you want to read (int)
 *
 * \return HIGH or LOW
 */
int digitalRead( uint32_t ulPin )
{  int v;
// todo
//    todo(__FUNCTION__);
	v = _read( ulPin);	
	return v;
}


void test(void)
{
	int i;
	i = sizeof(DigitalInOut);
	i = sizeof(pio);
//  _set_pin_output(0x48);

	i = sizeof(pio[1]);
}

int _enY=1, _dirY=0;
unsigned int t_step_plus_0 = 0,  t_step_plus_1 = 0,   dt_step_plus0=0, dt_step_plus1=0;
unsigned int t_step_minus_0 = 0,  t_step_minus_1 = 0, dt_step_minus0=0,dt_step_minus1=0;

void _write(uint32_t io, int v)
{
  if(io >= 32*5)
	{	err3(__FUNCTION__, " io >= 32*5", io);
   	return;  //do nothing
	}
	if(!pio[io].is_inited())
	{
		err3(__FUNCTION__, " Pin not inited", io);
   	return;  //do nothing
	}
//	if(io == _IPIN(1,27)) // ENY  P1_27
//	{  	pio[LED_PIN] = v;
//	}
#if 0	
	if(io == _IPIN(1,18)) // ENY  P1_27
	{  _enY = v;
	} else
	if(io == _IPIN(1,20)) 
	{  _dirY = v;
	} else
	if(io ==  _IPIN(1,19))// P1_28 StepY
	{  	
		 if(!_enY)
		 { if(_dirY)
			 { if(v)
				 {  t_step_plus_0 = LPC_TIM3->TC;
					 dt_step_plus1 = t_step_plus_0 - t_step_plus_1;
				 } else {
//					 t_step_plus_1 = LPC_TIM3->TC;
//					 while(LPC_TIM3->TC - t_step_plus_1 < 200);
					 t_step_plus_1 = LPC_TIM3->TC;
					 
					 dt_step_plus0 = t_step_plus_1 - t_step_plus_0;
				 }
			 } else {
			   if(v)
				 {  t_step_minus_0 = LPC_TIM3->TC;
					 dt_step_minus1 = t_step_minus_0 - t_step_minus_1;
				 } else {
//					 t_step_minus_1 = LPC_TIM3->TC;
//					 while(LPC_TIM3->TC - t_step_minus_1 < 200);
					 t_step_minus_1 = LPC_TIM3->TC;
					 dt_step_minus0 = t_step_minus_1 - t_step_minus_0;
				 }
			 }
		 }

		
		pio[LED_PIN] = v;
	}
	if(io == _IPIN(2,8)) // 
	{  	pio[LED_PIN] = v;
	}
#endif //0	
	pio[io] = v;
	return;
}

extern void  SetCheckHeater(int onoff);

#if HEATER_PIN_EXIST
void HeaterOn(int onOff)
{			pio[HEATER_PIN] = onOff;
#if defined(USE_TMP03)
	    SetCheckHeater(onOff); //reset 
#endif
}

#endif

#if RED_LASER_PIN_EXIST
void ReadLaserOn(int onOff)
{			pio[RED_LASER_PIN] = onOff;
}
#endif

#if ZATVOR_PIN_EXIST
void ZatvorOn(int onOff)
{			pio[ZATVOR_PIN] = onOff;
}
#endif

#if LED_PIN_EXIST
void LedOn(int onOff)
{			pio[LED_PIN] = onOff;
}
#endif

#if LED1_PIN_EXIST
void Led1On(int onOff)
{			pio[LED1_PIN] = onOff;
}

#endif

int _read(int io)
{  int v;
  if(io >= 32*5)
	{	err3(__FUNCTION__, " io >= 32*5", io);
   	return 0;  //do nothing
	}
	if(!pio[io].is_inited())
	{
		err3(__FUNCTION__, " Pin not inited", io);
   	return 0;  //do nothing
	}

	#if defined(EMULATOR)
extern volatile long  native_count_position[NUM_AXIS];
extern int tmp_flag;
   
	switch(io)
		 {case X_MAX_PIN:
			 v = LOW;
			 if(tmp_flag <= 0)
				 tmp_flag--;
#if X_MAX_ENDSTOP_INVERTING
			 if(native_count_position[X_AXIS] <= EMU_XMAXPOS)
			 {	 v = HIGH;
			 } else { 
			 if(tmp_flag <= 0)
				 tmp_flag--;				 
			 }
			 
#else			 
			 if(native_count_position[X_AXIS] > EMU_XMAXPOS) v = HIGH;
#endif			 			 
					break;
			case X_MIN_PIN:
			 v = LOW;
#if X_MIN_ENDSTOP_INVERTING
			 if(native_count_position[X_AXIS] >= EMU_XMINPOS)
			 {	 v = HIGH;
			 } else { 
			 if(tmp_flag <= 0)
				 tmp_flag--;				 
			 }
#else			 
			 if(native_count_position[X_AXIS] < EMU_XMINPOS) v = HIGH;
#endif			 
			
					break;
			case Y_MAX_PIN:
			 v = LOW;
#if Y_MAX_ENDSTOP_INVERTING
			 if(native_count_position[Y_AXIS] <= EMU_YMAXPOS) v = HIGH;
#else			 
			 if(native_count_position[Y_AXIS] > EMU_YMAXPOS) v = HIGH;
#endif			 			 
					break;
			case Y_MIN_PIN:
			 v = LOW;
#if Y_MIN_ENDSTOP_INVERTING
			 if(native_count_position[Y_AXIS] >= EMU_YMINPOS) v = HIGH;
#else			 
			 if(native_count_position[Y_AXIS] < EMU_YMINPOS) v = HIGH;
#endif			 
					break;
			default:
			v = pio[io].read();
			return v;
		}
#else		

	//test
//	{  int lon = 0, v=0;
//		 v =  pio[X_STOP_PIN].read();
//		 if(v == 0) 
//			 v =  pio[Y_STOP_PIN].read();
//		 if(v == 0)  v =  pio[Z_STOP_PIN].read();
//			pio[LED_PIN] = v;
//	}
	return pio[io].read();
#endif		
}

/*****************************************************/
/*  setup pin to out,input or input pullup
    dwPin = pin number 0 - (32*5 - 1) = P0.0 - P4.31
*/
void pinMode(uint32_t  dwPin, uint32_t dwMode ) 
{
#define INPUT  0
#define INPUT_PULLUP 1
#define OUTPUT 2
	PinName  pin;
	uint32_t upin0;
//tst
  if(dwPin == 52)
	  printf("52");
//  printf("Pin %i.%i %i\n",dwPin/32, dwPin%32,dwMode);

//check
  if(dwPin >= 32*5)
			return;  //do nothing
	upin0 = (uint32_t) P0_0;
	pin = (PinName) (upin0 + dwPin);
	if(!pio[dwPin].is_inited())
	{ 	pio[dwPin] = DigitalInOut(pin);
	}
	if(dwMode == OUTPUT)
	{
      	pio[dwPin].output();
//        gpio_init(&gpio, pin, PIN_OUTPUT);
		
	} else {  /* INPUT  0 or INPUT_PULLUP */
      	pio[dwPin].input();
//        gpio_init(&gpio, pin, PIN_INPUT);
				if(dwMode == INPUT_PULLUP)
						pio[dwPin].mode(PullUp); //		gpio_mode(&gpio, PullUp);		
	}
	return;
}
/*****************************************************/
void _set_pin_output(uint32_t io)
{ // gpio_t gpio;
	uint32_t upin0;
	PinName  pin;
	
  if(io >= 32*5)
	{	err2(__FUNCTION__, " io >= 32*5");
   	return;  //do nothing
	}
	upin0 = (uint32_t) P0_0;
	pin = (PinName) (upin0 + io);
	pio[io] = DigitalInOut(pin);
//Set as an output	
	pio[io].output();
//   gpio_init(&pio[io], pin, PIN_OUTPUT);
//    todo(__FUNCTION__);
	return;
}

void _set_pin_input(uint32_t  io)
{
//	gpio_t gpio;
	PinName  pin;
	uint32_t upin0;
  if(io >= 32*5)
	{	err2(__FUNCTION__, " io >= 32*5");
   	return;  //do nothing
	}
//	pin = (PinName) io;
//   gpio_init(&gpio, pin, PIN_INPUT);
	upin0 = (uint32_t) P0_0;
	pin = (PinName) (upin0 + io);
	pio[io] = DigitalInOut(pin);
//Set as an input	
	pio[io].input();
	
	return;
}



#include "LPC1768Serial.h"
extern LPC1768Serial customizedSerial;

//extern "C" { void HardFault_Handler (void); }

//void HardFault_Handler (void)
//{
//   customizedSerial.print("Пипец.HardFault_Handler\n");
//}


void todo(const char *str)
{
	customizedSerial.write_0("todo ");
	customizedSerial.write_0(str);
	customizedSerial.write_0("\n");
}

void err2(const char *str, const char *str1)
{
	customizedSerial.write_0(str);
	customizedSerial.write_0(str1);
	customizedSerial.write_0("\n");
}

void err3(const char *str, const char *str1, int par)
{  char tmp[40];
	 sprintf(tmp," x%x", par); 
	customizedSerial.write_0(str);
	customizedSerial.write_0(str1);
	customizedSerial.write_0(tmp);
	customizedSerial.write_0("\n");
}

void test00(void)
{  unsigned int t0;
	//LPC_TIM3->TC
	_set_pin_output(LED_PIN);
	_write(LED_PIN, 0);
	_set_pin_output(Y_STEP_PIN);
	_write(Y_STEP_PIN, 0);
	_set_pin_output(Y_ENABLE_PIN);
	_write(Y_ENABLE_PIN, 0);
	_set_pin_output(Y_DIR_PIN);
	_write(Y_DIR_PIN, 0);
	_write(Y_ENABLE_PIN, 0);
	_write(Y_DIR_PIN, 0);
	do
	{
	t0 =LPC_TIM3->TC;
	while(LPC_TIM3->TC - t0 < 85);
	_write(LED_PIN, 1);
	_write(Y_STEP_PIN, 1);
	t0 =LPC_TIM3->TC;
	while(LPC_TIM3->TC - t0 < 285);
	_write(LED_PIN, 0);
	_write(Y_STEP_PIN, 0);
		
	} while(1);
}

int median_of_3(int a, int b, int c)
{
	int middle;
	if( (a <= b) && (a <= c)) {
		middle = ( b <= c) ? b : c;
	} else {
		if( (b <= a) && (b <= c)) {
			middle = (a <= c) ? a : c;
		} else {
			middle = (a <= b) ? a : b;
		}
	}
	return middle;
}	
#endif // __ARM_LPC1768__
