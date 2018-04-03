/* master.c Cortex Tro(itsk powder) */

#include "LPC17xx.h"      // Keil: Register definition file for LPC17xx
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef WIN32
	#include "SiM.h"
#else
	#include <RTL.h>
	#include <Net_Config.h>
#endif // WIN32

/* external */

extern void SystemInit (void);
extern void Stop(void);

extern void stol_4_Init(void);
extern U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len);
extern U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par);
extern U16 tcp1_callback (U8 soc, U8 evt, U8 *ptr, U16 par);
extern void UdpSend(void);
extern void TcpSend(void);

extern void setupTimers(void);
extern void setupUart(void);
extern void setup(void);
extern void loop(void);
extern void test00(void);

/* internal */
void net_stuff(void);
int mystuff(int nolink);
int needEndWeb(void);
void QEI_init(void);
void LPCInit(void);

int need_EndWeb = 0;

#ifdef WIN32
//emulator
extern int SiM(void);
extern int SiMStolInit(void);
extern int SysTick_Config(unsigned int sclkperiod);  

int mystuff(int nolink);
int main_TcpNet(void);
int console_echo_on = 0;

void myInit(int state)
{  
	console_echo_on = state;
//   SystemInit();                                      /* setup core clocks */
   SysTick_Config(SystemCoreClock/1000);      // start millisecond global counter
	
	LPCInit();
    setupTimers();

	 setupUart();
QEI_init();	
	 setup();

}

int myMainCircle(void)
{
	SiMStolInit(); 
  while (1) {
//    timer_poll ();
    main_TcpNet ();
    UdpSend();
	SiM();
	mystuff(0);
 }
  return 0;
}

void net_stuff(void)
{
//    timer_poll ();
    main_TcpNet ();
	  UdpSend();
    TcpSend();
}


int needEndWeb(void)
{  return need_EndWeb;
}


#else

int main(void)
{ int  rc;

M0:
//  NeedInit = 0;
 // SystemInit(); // вызывается до main()                                      /* setup core clocks */
//  SysTick_Config(SystemFrequency/100);               /* Generate interrupt every 10 ms */
//  LPCInit();
/* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
//test	
//	stol_4_Init();

	
//0x26013f37 - lpc1768 0x25013f37 = lpc 1758
  // rc = iapReadPartID();
	
//	test00();
   setupTimers();
	 setupUart();
	 setup();
	do
	{
	 loop();
	} while(1);
}

#endif // WIN32

#if 0

/////////////////////////////////////////////////////////////////////////////

/*--------------------------- dhcp_check ------------------------------------*/
#if DHCP_ENABLE

static void dhcp_check (void) 
{
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }
  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;
//    sprintf((char *)lcd_text[0]," IP address:");
//    sprintf((char *)lcd_text[1]," %d.%d.%d.%d", MY_IP[0], MY_IP[1],
//                                                MY_IP[2], MY_IP[3]);
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
    dhcp_disable ();
//    sprintf((char *)lcd_text[1]," DHCP failed    " );
    dhcp_tout = 30 | 0x80000000;
    return;
  }
  if (dhcp_tout == 0x80000000) {
    dhcp_tout = 0;
//    sprintf((char *)lcd_text[0]," IP address:");
//    sprintf((char *)lcd_text[1]," %d.%d.%d.%d", MY_IP[0], MY_IP[1],
//                                                MY_IP[2], MY_IP[3]);
  }
}
#endif //DHCP_ENABLE
/*--------------------------- timer_poll ------------------------------------*/
BOOL tick;
int tick_100ms=0;

static void timer_poll0 (void) {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
		tick_100ms++;
    tick = __TRUE;
  }
}

static void timer_poll (void) {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}


#if 0
void LPCInit(void)
{
//UART0 P0.19 - Выбор контроллера
/* P0.2, P0.3 = TxD RxD (UART0) 0101 0000*/
  LPC_PINCON->PINSEL0 |= 0x00000050;
  LPC_UART0->IER=0x00; //NO IRQ
  LPC_UART0->FCR=0x01; //FIFO reset
  LPC_UART0->LCR=0x83;
//Baudrate=62500000
  LPC_UART0->DLM=0x00;
  LPC_UART0->DLL=0x01;	//0x02; 3125000 //0x01; =62500000
  LPC_UART0->LCR=0x03;


// Keil: Timer 0 is used for TCP retransmission control, init at void TCPLowLevelInit(void)
/* таймер T0 */
//??  T0TCR=0x01;
/* таймер T1 */
  LPC_TIM1->TCR=1;

/* таймер T2 */ //прерывания
/*   LPC_TIM2->MR0 = 250000;                            // 10000 mSec
   LPC_TIM2->MCR = 3;                                  // Interrupt and Reset on MR0
   LPC_TIM2->TCR=  0x01;
   NVIC_EnableIRQ(TIMER2_IRQn);
*/

// P2.0-P2.11 -> Out; Выходы  (вообще-то определяются в LAMP_DEFINExx )
/* P2.4 = PWM  */

  LPC_GPIO2->FIOSET = 0x0fef; //0x0fff;
  LPC_GPIO2 -> FIODIR |= 0x00000fef; //0x00000fff;
  
/* P2.4 = PWM1.5 = 0001 0000 0000*/
  LPC_PINCON->PINSEL4 |= 0x0000100;
//  LPC_PWM1->PCR = 0x2000;	//PWM Control Register - The PWM5 output enabled, single edge mode
//  LPC_PWM1->MCR = 0x00000002;  //PWM Match Control Register, The MCR is used to control if an interrupt is generated and if the TC is reset when a Match occurs.
//  LPC_PWM1->MR0 = PWM_T0;  
//  LPC_PWM1->MR5 = PWM_T0/2;
//  LPC_PWM1->LER = 0x7F;
//  LPC_PWM1->TCR = 0x0B;//PWM Timer Control Register 1011 PWM Enable|Reserved|Counter Reset| Counter Enable
//  LPC_PWM1->TCR = 0x09;//1001       PWM Enable|0|0|Counter Enable

/* настройка SSP порта: (2-датчика слежения) */
/* SSP1 живет на ноге P0.7 SCK1  P0.6 SSEL0 P?0.8 MISO0 P0.9 MOIS0 */

/* P0.7, P0.6  P0.8 P0.9 = 1010 1010 0000 0000 0000 */
  LPC_PINCON->PINSEL0 |= 0x0000aa000;
  LPC_SSP1->CR0 = 0x0f;	//0x40 - полярность 0xf - 16 бит
  LPC_SSP1->CR1 = 0x00; // SSP1 disable
  LPC_SSP1->CR1 = 0x0e; // SSP1 enable+slave+SlaveOutput disabled	 2+4+8
//  LAMP_ON;

}

#endif
volatile int TimeTick  = 0;

/* SysTick interrupt happens every 10 ms */
void SysTick_Handler (void) {
  TimeTick++;
  if (TimeTick >= 20) {
    TimeTick = 0;
  }  
}

#endif //0


void LPCInit(void)
{
}

int mystuff(int nolink)
{ 
   loop();
   return 0;
}

/************************ QEI stuff *********************************************/
void QEI_init(void)
{
//     <o11.18>     PCQEI: Quadrature encoder interface power/clock enable
  LPC_SC->PCONP |= (1 << 18);      // Enable CLOCK for internal ADC controller

//P1.20, 1.23 (1.24) Encoder A,B, (Z) (index) MCFB0,MCFB1,MCFB2 
/*****************************************/
//9-8, 	15-14, (17-16)
//	(01) 01 xx xx 01 0000 0000
	
  LPC_PINCON->PINSEL3 |=0x00004100;
  //
  LPC_QEI->QEIMAXPOS = 0xffffffff;
  LPC_QEI->QEICONF = 0x04;
  LPC_QEI->FILTER = 0x10;
  LPC_QEI->QEIIES = 0x10;
  NVIC_EnableIRQ(QEI_IRQn);

}

int errXpos = 0, t0_err=0;
//Xpos = LPC_QEI->QEIPOS;

void QEI_IRQHandler (void) 
{  
  LPC_QEI->QEICLR = 0x0010; /* clear interrupt flag */
  errXpos++;
  t0_err = LPC_TIM1->TC;
  return;
}
/*********************************************************************/

