/* system_LPC17xx.c */
#include "LPC17xx.h"

#define __CORE_CLK 100000000
uint32_t SystemFrequency = __CORE_CLK;
/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __CORE_CLK;/*!< System Clock Frequency (Core Clock)*/

LPC_GPIO_TypeDef LPC_gpio0 = {0,0};
LPC_GPIO_TypeDef LPC_gpio1 = {0,0};
LPC_GPIO_TypeDef LPC_gpio2 = {0,0};
LPC_GPIO_TypeDef LPC_gpio3 = {0,0};
LPC_GPIO_TypeDef LPC_gpio4 = {0,0};
LPC_TIM_TypeDef LPC_tim0 = {0,0};
LPC_TIM_TypeDef LPC_tim1 = {0,0};
LPC_TIM_TypeDef LPC_tim2 = {0,0};
LPC_TIM_TypeDef LPC_tim3 = {0,0};
LPC_PWM_TypeDef LPC_pwm1 = {0,0};
LPC_PINCON_TypeDef LPC_pincon = {0,0};
LPC_ADC_TypeDef LPC_adc = {0,0};
LPC_SC_TypeDef LPC_sc = {0,0};
LPC_UART_TypeDef LPC_uart0  = {0,0};
LPC_UART_TypeDef LPC_uart1 = {0,0};
LPC_UART_TypeDef LPC_uart2 = {0,0};
LPC_UART_TypeDef LPC_uart3 = {0,0};
LPC_QEI_TypeDef  LPC_qei = {0,0};

int  irqs_is_enabled = 1;

void __disable_irq(void)
{
	irqs_is_enabled = 0;
	//todo
}

void __enable_irq(void)
{
	irqs_is_enabled = 1;
	//todo
}



