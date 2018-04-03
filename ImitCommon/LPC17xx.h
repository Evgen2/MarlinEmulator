/* LPC17xx.h */
#ifndef __LPC1768_H 
#define __LPC1768_H

#define  __IO
#define  __O
#define  __I

typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;
typedef unsigned int U32;
typedef int          BOOL;

/** @brief IRQ interrupt source definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  Reset_IRQn                    = -15,      /*!< 1 Reset Vector, invoked on PowerUp and warm reset*/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  HardFault_IRQn                = -13,      /*!< 3  Hard Fault, all classes of Fault              */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
  WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
  SPI_IRQn                      = 13,       /*!< SPI Interrupt                                    */
  SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
  RIT_IRQn                      = 29,       /*!< Repetitive Interrupt Timer Interrupt             */
  MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
  USBActivity_IRQn              = 33,       /*!< USB Activity Interrupt 						  */
  CANActivity_IRQn              = 34,       /*!< CAN Activity Interrupt 						  */
} IRQn_Type;


/*------------- Pin Connect Block (PINCON) -----------------------------------*/
/** @brief Pin Connect Block (PINCON) register structure definition */
typedef struct
{
  __IO uint32_t PINSEL0;
  __IO uint32_t PINSEL1;
  __IO uint32_t PINSEL2;
  __IO uint32_t PINSEL3;
  __IO uint32_t PINSEL4;
  __IO uint32_t PINSEL5;
  __IO uint32_t PINSEL6;
  __IO uint32_t PINSEL7;
  __IO uint32_t PINSEL8;
  __IO uint32_t PINSEL9;
  __IO uint32_t PINSEL10;
       uint32_t RESERVED0[5];
  __IO uint32_t PINMODE0;
  __IO uint32_t PINMODE1;
  __IO uint32_t PINMODE2;
  __IO uint32_t PINMODE3;
  __IO uint32_t PINMODE4;
  __IO uint32_t PINMODE5;
  __IO uint32_t PINMODE6;
  __IO uint32_t PINMODE7;
  __IO uint32_t PINMODE8;
  __IO uint32_t PINMODE9;
  __IO uint32_t PINMODE_OD0;
  __IO uint32_t PINMODE_OD1;
  __IO uint32_t PINMODE_OD2;
  __IO uint32_t PINMODE_OD3;
  __IO uint32_t PINMODE_OD4;
  __IO uint32_t I2CPADCFG;
} LPC_PINCON_TypeDef;

// version of PINCON_TypeDef using register arrays
typedef struct {
  __IO uint32_t PINSEL[11];
       uint32_t RESERVED0[5];
  __IO uint32_t PINMODE[10];
  __IO uint32_t PINMODE_OD[5];
} PINCONARRAY_TypeDef;


/*------------- General Purpose Input/Output (GPIO) --------------------------*/
/** @brief General Purpose Input/Output (GPIO) register structure definition */
typedef struct
{
  union {
    __IO uint32_t FIODIR;
    struct {
      __IO uint16_t FIODIRL;
      __IO uint16_t FIODIRH;
    };
    struct {
      __IO uint8_t  FIODIR0;
      __IO uint8_t  FIODIR1;
      __IO uint8_t  FIODIR2;
      __IO uint8_t  FIODIR3;
    };
  };
  uint32_t RESERVED0[3];
  union {
    __IO uint32_t FIOMASK;
    struct {
      __IO uint16_t FIOMASKL;
      __IO uint16_t FIOMASKH;
    };
    struct {
      __IO uint8_t  FIOMASK0;
      __IO uint8_t  FIOMASK1;
      __IO uint8_t  FIOMASK2;
      __IO uint8_t  FIOMASK3;
    };
  };
  union {
    __IO uint32_t FIOPIN;
    struct {
      __IO uint16_t FIOPINL;
      __IO uint16_t FIOPINH;
    };
    struct {
      __IO uint8_t  FIOPIN0;
      __IO uint8_t  FIOPIN1;
      __IO uint8_t  FIOPIN2;
      __IO uint8_t  FIOPIN3;
    };
  };
//FIOSET и FIOCLR заменяем дефайнами
/* */
  union {
    __IO uint32_t FIOSET;
    struct {
      __IO uint16_t FIOSETL;
      __IO uint16_t FIOSETH;
    };
    struct {
      __IO uint8_t  FIOSET0;
      __IO uint8_t  FIOSET1;
      __IO uint8_t  FIOSET2;
      __IO uint8_t  FIOSET3;
    };
  };

  union {
    __O  uint32_t FIOCLR;
    struct {
      __O  uint16_t FIOCLRL;
      __O  uint16_t FIOCLRH;
    };
    struct {
      __O  uint8_t  FIOCLR0;
      __O  uint8_t  FIOCLR1;
      __O  uint8_t  FIOCLR2;
      __O  uint8_t  FIOCLR3;
    };
  };
/* */
} LPC_GPIO_TypeDef;


/*------------- Timer (TIM) --------------------------------------------------*/
/** @brief Timer (TIM) register structure definition */
typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
  __I  uint32_t CR1;
       uint32_t RESERVED0[2];
  __IO uint32_t EMR;
       uint32_t RESERVED1[12];
  __IO uint32_t CTCR;
} LPC_TIM_TypeDef;

/*------------- Pulse-Width Modulation (PWM) ---------------------------------*/
/** @brief Pulse-Width Modulation (PWM) register structure definition */
typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
  __I  uint32_t CR1;
  __I  uint32_t CR2;
  __I  uint32_t CR3;
       uint32_t RESERVED0;
  __IO uint32_t MR4;
  __IO uint32_t MR5;
  __IO uint32_t MR6;
  __IO uint32_t PCR;
  __IO uint32_t LER;
       uint32_t RESERVED1[7];
  __IO uint32_t CTCR;
} LPC_PWM_TypeDef;

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
/** @brief  Analog-to-Digital Converter (ADC) register structure definition */
typedef struct
{
  __IO uint32_t ADCR;
  __IO uint32_t ADGDR;
       uint32_t RESERVED0;
  __IO uint32_t ADINTEN;
  __I  uint32_t DR[8];                      /*!< Offset: 0x010 (R/ )  A/D Channel # Data Register */
/*	
  __I  uint32_t ADDR0;
  __I  uint32_t ADDR1;
  __I  uint32_t ADDR2;
  __I  uint32_t ADDR3;
  __I  uint32_t ADDR4;
  __I  uint32_t ADDR5;
  __I  uint32_t ADDR6;
  __I  uint32_t ADDR7;
*/	
  __I  uint32_t ADSTAT;
  __IO uint32_t ADTRM;
} LPC_ADC_TypeDef;

/*------------- System Control (SC) ------------------------------------------*/
/** @brief System Control (SC) register structure definition */
typedef struct
{
  __IO uint32_t FLASHCFG;               /* Flash Accelerator Module           */
       uint32_t RESERVED0[31];
  __IO uint32_t PLL0CON;                /* Clocking and Power Control         */
  __IO uint32_t PLL0CFG;
  __I  uint32_t PLL0STAT;
  __O  uint32_t PLL0FEED;
       uint32_t RESERVED1[4];
  __IO uint32_t PLL1CON;
  __IO uint32_t PLL1CFG;
  __I  uint32_t PLL1STAT;
  __O  uint32_t PLL1FEED;
       uint32_t RESERVED2[4];
  __IO uint32_t PCON;
  __IO uint32_t PCONP;
       uint32_t RESERVED3[15];
  __IO uint32_t CCLKCFG;
  __IO uint32_t USBCLKCFG;
  __IO uint32_t CLKSRCSEL;
  __IO uint32_t	CANSLEEPCLR;
  __IO uint32_t	CANWAKEFLAGS;
       uint32_t RESERVED4[10];
  __IO uint32_t EXTINT;                 /* External Interrupts                */
       uint32_t RESERVED5;
  __IO uint32_t EXTMODE;
  __IO uint32_t EXTPOLAR;
       uint32_t RESERVED6[12];
  __IO uint32_t RSID;                   /* Reset                              */
       uint32_t RESERVED7[7];
  __IO uint32_t SCS;                    /* Syscon Miscellaneous Registers     */
  __IO uint32_t IRCTRIM;                /* Clock Dividers                     */
  __IO uint32_t PCLKSEL0;
  __IO uint32_t PCLKSEL1;
       uint32_t RESERVED8[4];
  __IO uint32_t USBIntSt;               /* USB Device/OTG Interrupt Register  */
  __IO uint32_t DMAREQSEL;
  __IO uint32_t CLKOUTCFG;              /* Clock Output Configuration         */
 } LPC_SC_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
/** @brief  Universal Asynchronous Receiver Transmitter (UART) register structure definition */
typedef struct
{
  union {
  __I  uint8_t  RBR;
  __O  uint8_t  THR;
  __IO uint8_t  DLL;
       uint32_t RESERVED0;
  };
  union {
  __IO uint8_t  DLM;
  __IO uint32_t IER;
  };
  union {
  __I  uint32_t IIR;
  __O  uint8_t  FCR;
  };
  __IO uint8_t  LCR;
       uint8_t  RESERVED1[7];
  __I  uint8_t  LSR;
       uint8_t  RESERVED2[7];
  __IO uint8_t  SCR;
       uint8_t  RESERVED3[3];
  __IO uint32_t ACR;
  __IO uint8_t  ICR;
       uint8_t  RESERVED4[3];
  __IO uint8_t  FDR;
       uint8_t  RESERVED5[7];
  __IO uint8_t  TER;
} LPC_UART_TypeDef;

/** @brief  Controller Area Network Controller (CAN) register structure definition */
typedef struct                          /* Controller Registers               */
{
  __IO uint32_t MOD;
  __O  uint32_t CMR;
  __IO uint32_t GSR;
  __I  uint32_t ICR;
  __IO uint32_t IER;
  __IO uint32_t BTR;
  __IO uint32_t EWL;
  __I  uint32_t SR;
  __IO uint32_t RFS;
  __IO uint32_t RID;
  __IO uint32_t RDA;
  __IO uint32_t RDB;
  __IO uint32_t TFI1;
  __IO uint32_t TID1;
  __IO uint32_t TDA1;
  __IO uint32_t TDB1;
  __IO uint32_t TFI2;
  __IO uint32_t TID2;
  __IO uint32_t TDA2;
  __IO uint32_t TDB2;
  __IO uint32_t TFI3;
  __IO uint32_t TID3;
  __IO uint32_t TDA3;
  __IO uint32_t TDB3;
} LPC_CAN_TypeDef;

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
/** @brief  Synchronous Serial Communication (SSP) register structure definition */
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __I  uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
} LPC_SSP_TypeDef;


/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
/** @brief  Inter-Integrated Circuit (I2C) register structure definition */
typedef struct
{
  __IO uint32_t I2CONSET;
  __I  uint32_t I2STAT;
  __IO uint32_t I2DAT;
  __IO uint32_t I2ADR0;
  __IO uint32_t I2SCLH;
  __IO uint32_t I2SCLL;
  __O  uint32_t I2CONCLR;
  __IO uint32_t MMCTRL;
  __IO uint32_t I2ADR1;
  __IO uint32_t I2ADR2;
  __IO uint32_t I2ADR3;
  __I  uint32_t I2DATA_BUFFER;
  __IO uint32_t I2MASK0;
  __IO uint32_t I2MASK1;
  __IO uint32_t I2MASK2;
  __IO uint32_t I2MASK3;
} LPC_I2C_TypeDef;


/*------------- Quadrature Encoder Interface (QEI) ---------------------------*/
/** @brief  Quadrature Encoder Interface (QEI) register structure definition */
typedef struct
{
  __O  uint32_t QEICON;
  __I  uint32_t QEISTAT;
  __IO uint32_t QEICONF;
  __I  uint32_t QEIPOS;
  __IO uint32_t QEIMAXPOS;
  __IO uint32_t CMPOS0;
  __IO uint32_t CMPOS1;
  __IO uint32_t CMPOS2;
  __I  uint32_t INXCNT;
  __IO uint32_t INXCMP;
  __IO uint32_t QEILOAD;
  __I  uint32_t QEITIME;
  __I  uint32_t QEIVEL;
  __I  uint32_t QEICAP;
  __IO uint32_t VELCOMP;
  __IO uint32_t FILTER;
       uint32_t RESERVED0[998];
  __O  uint32_t QEIIEC;
  __O  uint32_t QEIIES;
  __I  uint32_t QEIINTSTAT;
  __I  uint32_t QEIIE;
  __O  uint32_t QEICLR;
  __O  uint32_t QEISET;
} LPC_QEI_TypeDef;



#ifdef __cplusplus
extern "C" {
#endif
extern LPC_GPIO_TypeDef LPC_gpio0;
extern LPC_GPIO_TypeDef LPC_gpio1;
extern LPC_GPIO_TypeDef LPC_gpio2;
extern LPC_GPIO_TypeDef LPC_gpio3;
extern LPC_GPIO_TypeDef LPC_gpio4;
extern LPC_TIM_TypeDef LPC_tim0;
extern LPC_TIM_TypeDef LPC_tim1;
extern LPC_TIM_TypeDef LPC_tim2;
extern LPC_TIM_TypeDef LPC_tim3;
extern LPC_PWM_TypeDef LPC_pwm1;
extern LPC_PINCON_TypeDef LPC_pincon;
extern LPC_ADC_TypeDef LPC_adc;
extern LPC_SC_TypeDef LPC_sc;
extern LPC_QEI_TypeDef LPC_qei;
extern LPC_UART_TypeDef LPC_uart0;
extern LPC_UART_TypeDef LPC_uart1;
extern LPC_UART_TypeDef LPC_uart2;
extern LPC_UART_TypeDef LPC_uart3;

#define LPC_GPIO0             ((LPC_GPIO_TypeDef  *) &LPC_gpio0  )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef  *) &LPC_gpio1  )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef  *) &LPC_gpio2  )
#define LPC_GPIO3             ((LPC_GPIO_TypeDef  *) &LPC_gpio3  )
#define LPC_GPIO4             ((LPC_GPIO_TypeDef  *) &LPC_gpio4  )
#define LPC_GPIO0_FIOSET(v)  LPC_gpio0.FIOPIN |= v 
#define LPC_GPIO1_FIOSET(v)  LPC_gpio1.FIOPIN |= v 
#define LPC_GPIO2_FIOSET(v)  LPC_gpio2.FIOPIN |= v 
#define LPC_GPIO3_FIOSET(v)  LPC_gpio3.FIOPIN |= v 

#define LPC_GPIO0_FIOCLR(v)  LPC_gpio0.FIOPIN &= (~v) 
#define LPC_GPIO1_FIOCLR(v)  LPC_gpio1.FIOPIN &= (~v) 
#define LPC_GPIO2_FIOCLR(v)  LPC_gpio2.FIOPIN &= (~v) 
#define LPC_GPIO3_FIOCLR(v)  LPC_gpio3.FIOPIN &= (~v) 
//	LPC_gpio0.FIOPIN &= (~v);
//	LPC_gpio0.FIOPIN |= v;

#define LPC_TIM0              ((LPC_TIM_TypeDef  *) &LPC_tim0)
#define LPC_TIM1              ((LPC_TIM_TypeDef  *) &LPC_tim1)
#define LPC_TIM2              ((LPC_TIM_TypeDef  *) &LPC_tim2)
#define LPC_TIM3              ((LPC_TIM_TypeDef  *) &LPC_tim3)
#define LPC_PWM1              ((LPC_PWM_TypeDef  *) &LPC_pwm1)
#define LPC_PINCON            ((LPC_PINCON_TypeDef *) &LPC_pincon )
#define PINCONARRAY ((PINCONARRAY_TypeDef *)&LPC_pincon)

#define LPC_ADC               ((LPC_ADC_TypeDef *) &LPC_adc)
#define LPC_ADC               ((LPC_ADC_TypeDef *) &LPC_adc)
#define LPC_SC                ((LPC_SC_TypeDef *)  &LPC_sc)

#define LPC_QEI               ((LPC_QEI_TypeDef *) &LPC_qei)
#define LPC_UART0             ((LPC_UART_TypeDef *) &LPC_uart0)
#define LPC_UART1             ((LPC_UART_TypeDef *) &LPC_uart1)
#define LPC_UART2             ((LPC_UART_TypeDef *) &LPC_uart2)
#define LPC_UART3             ((LPC_UART_TypeDef *) &LPC_uart3)

//todo
#define LPC_GPIO0_BASE 0
#define LPC_UART0_BASE 1 
#define LPC_UART1_BASE 2 
#define LPC_UART2_BASE 3 
#define LPC_UART3_BASE 4 
#define LPC_SSP0_BASE 0
#define LPC_SSP1_BASE 0
#define LPC_I2C0_BASE 0
#define LPC_I2C1_BASE 0
#define LPC_I2C2_BASE 0
#define LPC_CAN1_BASE 0
#define LPC_CAN2_BASE 0

#define TIMER1_IRQn   1
#define TIMER2_IRQn   2
#define ADC_IRQn  	3	
extern void NVIC_DisableIRQ(int irqn);
extern void NVIC_EnableIRQ(int irqn);
extern void NVIC_SetPendingIRQ(int irqn);
extern void NVIC_SetPriority(int irqn, int a);
extern int NVIC_EncodePriority(int a,int b,int c);
extern uint32_t SystemCoreClock;

extern void __disable_irq(void);
extern void __enable_irq(void);

#ifdef __cplusplus
}
#endif

#endif // __LPC1768_H 
