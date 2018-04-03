/* HAL_LPC1768_TMP03.cpp */
#include "../../config_0.h"
#ifdef __ARM_LPC1768__
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "mbed.h"
#include "wait_api.h"
#include "../HAL.h"
#include "../../inc/MarlinConfig.h"

#if defined(USE_TMP03)
extern "C" void EINT3_IRQHandler(void);
float TMP03_CoeffSensorA[2]={235., 235.};
float TMP03_CoeffSensorB[2]={-400., -400.};

static unsigned int tTmp_0r=0,tTmp_0f=0,tTmp_2r=0,tTmp_2f=0;

void HAL_TMP03_init(void)
{
//
  pinMode(TMP03_0_PIN, INPUT_PULLUP);
  pinMode(TMP03_1_PIN, INPUT_PULLUP);
/* датчики температуры живут на */
//P2.2  - t1  //D4
//P0.25 - t2  //D40
#ifndef WIN32	
   LPC_GPIOINT->IO0IntEnR = 0x02000000;
   LPC_GPIOINT->IO0IntEnF = 0x02000000;

   LPC_GPIOINT->IO2IntEnR = 0x00000004;
   LPC_GPIOINT->IO2IntEnF = 0x00000004;
#endif
   tTmp_0r = tTmp_0f = tTmp_2r = tTmp_2f = LPC_TIM1->TC;

	NVIC_EnableIRQ(EINT3_IRQn);  /* внешнее прерывание от GPIO0 и GPIO2 */

}

static int idT0cur[2]={0,0}, idT2cur[2]={0,0};
//static int dT0[2]={0,0}, dT2[2]={0,0};
int TMP03_idT0[2]={0,0}, TMP03_idT2[2]={0,0};
static int isNew0=0,isNew2=0;

void EINT3_IRQHandler(void)
{  int ti, needCount=0, r0,r2;
static int raz1=0, raz2=0;

#if defined(WIN32)
//todo
#else
   r0 = LPC_GPIOINT->IO0IntStatR | LPC_GPIOINT->IO0IntStatF;
		ti = LPC_TIM3->TC;

/* Температура  жидкости */
   if(r0)
   {
		if(LPC_GPIOINT->IO0IntStatR &  0x02000000)
		{   tTmp_0r = ti;
			  idT0cur[0] += tTmp_0r - tTmp_0f;
	      raz1++;	
				if(raz1 == 100)
				{ raz1 = 0;
					if(idT0cur[0])
					{ TMP03_idT0[1] = idT0cur[0]; // tmp 1<->0
						TMP03_idT0[0] = idT0cur[1];
						isNew0 = 1;
					}
					idT0cur[0] = idT0cur[1] = 0;
				}
			
		} else if(LPC_GPIOINT->IO0IntStatF &  0x02000000) {
		    tTmp_0f = ti;
        idT0cur[1] += tTmp_0f - tTmp_0r;
		}

	 }
			LPC_GPIOINT->IO0IntClr = r0;
/* Температура  воздуха */

   r2 = LPC_GPIOINT->IO2IntStatR | LPC_GPIOINT->IO2IntStatF;
   if(r2)
	 {
		if(LPC_GPIOINT->IO2IntStatR &  0x00000004)
		{ tTmp_2r = ti;
      idT2cur[0] += tTmp_2r - tTmp_2f;
			raz2++;
			if(raz2 == 100)
			{ if(idT2cur[0])
				{  	TMP03_idT2[1] = idT2cur[0]; //tmp 0<->1
						TMP03_idT2[0] = idT2cur[1];
						isNew2 = 1;
				}
				raz2 = 0;
				idT2cur[0] = idT2cur[1] = 0;
			}
			
		} else if(LPC_GPIOINT->IO2IntStatF &  0x00000004) {
			tTmp_2f = ti;
			idT2cur[1] += tTmp_2f - tTmp_2r;			
		}
			LPC_GPIOINT->IO2IntClr = r2;
	 }
#endif

}

#endif // defined(USE_TMP03)
#endif // __ARM_LPC1768__
