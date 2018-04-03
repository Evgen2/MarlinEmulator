/* SiM.cpp */
/* —имул€ци€ стола */
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LPC17xx.h"  
#include "Marlin.h"
#include "module\planner.h"
#include "module\temperature.h"

//todo #include "stol4_cmd0.h"
#include "SiM.hpp"  

class Stol_Sim StolSim;

extern "C" int SiM(void);
extern "C" void NVIC_EnableIRQ(int irqn);
extern "C" void NVIC_DisableIRQ(int irqn);
extern "C" void NVIC_SetPendingIRQ(int irqn); 
extern "C" void TIMER1_IRQHandler(void);
extern "C" void TIMER2_IRQHandler(void);
extern "C" void SysTick_Handler(void); 
extern "C" int SysTick_Config(unsigned int freq);  
extern "C" int mystuff(int par); 
extern int _read(int io);

int SimStart(void);
void TimerWork( void * param ); 
void MainLoopWork(void * param);

int GetSSPimit(void);

extern Temperature thermalManager;

int SiMreadCfg(void)
{ int rc;
   rc = StolSim.Read("MarlinImit.cfg");
   if(rc == 1)
	   StolSim.Write("MarlinImit.cfg",1);
   return rc;
}
int SiMwriteCfg(void)
{  int rc;
   rc = StolSim.Write("MarlinImit.cfg",1);
   return rc;

}

static int tTempclockOld = 0;
int SimStart(void)
{
	tTempclockOld = clock();
	StolSim.TempImitStart();
	return 0;
}

int SiM(void)
{ // StolSim.IncrTimer();
	StolSim.SetEndstops();
	return 0;
}

int Stol_Sim::SetEndstops(void)
{   int	 i, sts;
extern volatile long  native_count_position[NUM_AXIS];
extern Planner planner;
static int t0=0;
 for(i=0;i<4;i++) 
 {	x[i] = native_count_position[i] / planner.axis_steps_per_mm[i];
// if(i == 0)
//	 x[i] +=50.;
	if(axis[i].used)
	{  if(x[i] < axis[i].pos[0]) { native_count_position[i] = axis[i].pos[0]* planner.axis_steps_per_mm[i]; x[i] = axis[i].pos[0];}
	   if(x[i] > axis[i].pos[1]) { native_count_position[i] = axis[i].pos[1]* planner.axis_steps_per_mm[i]; x[i] = axis[i].pos[1];}
	}
 }
 if(clock()-t0 > 1000)
 { 
     extern volatile uint32_t _millis; //  millisecond global counter
	 t0 = clock();
	 printf("X %f Y %f Z %f F %f\n", x[0], x[1], x[2], x[3]);
	 //	printf(" %i %i %i %i\n",endstopMin[0].IsOn,endstopMax[0].IsOn,endstopMin[1].IsOn,endstopMax[1].IsOn);
 }
#if X_MIN_PIN_EXIST
	if(endstopMin[0].used)
	{	if(endstopMin[0].posOn[0] < x[0] &&  x[0] < endstopMin[0].posOn[1])
		{	endstopMin[0].IsOn = 1;
		} else {
			endstopMin[0].IsOn = 0;
		}
		sts = endstopMin[0].IsOn;
   #if X_MIN_ENDSTOP_INVERTING
		sts = (sts+1) & 0x01;
   #endif
		_write(X_MIN_PIN, sts);
	}
#endif 
#if Y_MIN_PIN_EXIST
	if(endstopMin[1].used)
	{	if(endstopMin[1].posOn[0] < x[1] &&  x[1] < endstopMin[1].posOn[1])
		{	endstopMin[1].IsOn = 1;
		} else {
			endstopMin[1].IsOn = 0;
		}
		sts = endstopMin[1].IsOn;
   #if Y_MIN_ENDSTOP_INVERTING
		sts = (sts+1) & 0x01;
   #endif
		_write(Y_MIN_PIN, sts);
	}
#endif 
#if Z_MIN_PIN_EXIST
	if(endstopMin[2].used)
	{	if(endstopMin[2].posOn[0] < x[2] &&  x[2] < endstopMin[2].posOn[1])
		{	endstopMin[2].IsOn = 1;
		} else {
			endstopMin[2].IsOn = 0;
		}
		sts = endstopMin[2].IsOn;
   #if Z_MIN_ENDSTOP_INVERTING
		sts = (sts+1) & 0x01;
   #endif
		_write(Z_MIN_PIN, sts);
	}
#endif 
#if X_MAX_PIN_EXIST
	if(endstopMax[0].used)
	{	if(endstopMax[0].posOn[0] < x[0] &&  x[0] < endstopMax[0].posOn[1])
		{	endstopMax[0].IsOn = 1;
		} else {
			endstopMax[0].IsOn = 0;
		}
		sts = endstopMax[0].IsOn;
   #if X_MAX_ENDSTOP_INVERTING
		sts = (sts+1) & 0x01;
   #endif
		_write(X_MAX_PIN, sts);
	}
#endif 
#if Y_MAX_PIN_EXIST
	if(endstopMax[1].used)
	{	if(endstopMax[1].posOn[0] < x[1] &&  x[1] < endstopMax[1].posOn[1])
		{	endstopMax[1].IsOn = 1;
		} else {
			endstopMax[1].IsOn = 0;
		}
		sts = endstopMax[1].IsOn;
   #if Y_MAX_ENDSTOP_INVERTING
		sts = (sts+1) & 0x01;
   #endif
		_write(Y_MAX_PIN, sts);
	}
#endif 
#if Z_MAX_PIN_EXIST
	if(endstopMax[2].used)
	{	if(endstopMax[2].posOn[0] < x[2] &&  x[1] < endstopMax[2].posOn[1])
		{	endstopMax[2].IsOn = 1;
		} else {
			endstopMax[2].IsOn = 0;
		}
		sts = endstopMax[2].IsOn;
   #if Z_MAX_ENDSTOP_INVERTING
		sts = (sts+1) & 0x01;
   #endif
		_write(Z_MAX_PIN, sts);
	}
#endif 

   return 0;
}

static int Traz=0, Tt0=0;
static double Tms=0.;
static int IRQen[10] ={0,0,0,0,0, 0,0,0,0,0};

int SetTimerIncr(int raz, int mc)
{    int rc;
     rc = StolSim.SetTimerIncr(raz, mc);
     return rc;
}

int Stol_Sim::SetTimerIncr(int raz, int mc)
{   double dt;
    dt = double(mc)/double(raz) * 1000.; //mks per step
#if 0
#endif //0
	if(dt > 250.) // медленне реального времени
		TimerKoeff = 25000000 * 0.000250;
	else 
		TimerKoeff = dt*25;
printf("TimerKoeff=%f\n", TimerKoeff);
	Traz = 0;
	Tt0 = clock();
	return 0;
}

int TimerCOn(void)
{
	Traz = 0;
	Tt0 = clock();
	return 0;
}


void MainLoopWork(void * param)
{
	while(1)
	{	 Sleep(1);
		 SiM();
		 mystuff(1);
	}
}

//нитка таймера
void TimerWork( void * param ) 
{  int i, n, t, dt0, dt, tclock0,tclock20, rc;
   static unsigned int t0, dtt;
   static unsigned int t20, dtt2;
   tclock0 = tclock20 = clock();
   LPC_TIM1->RESERVED0[0] = LPC_TIM0->TC;
   LPC_TIM2->RESERVED0[0] = LPC_TIM0->TC;
   LPC_TIM3->RESERVED0[0] = LPC_TIM0->TC;
   t0 = LPC_TIM0->TC;
   t20 = LPC_TIM2->TC;
   for(;;)
   {   Sleep(1);
//LPC_TIM1
   StolSim.SetEndstops(); //??
       t = clock();
	   StolSim.tclock = t;
	   dt0 = t - tclock0;
	   if(dt0 < 1)
		   continue;
	   if(dt0  > 500) dt0 = 500;  // если у нас были остановки в отладчике
	   tclock0 = t;
	   rc = StolSim.IncrTimer3(dt0*1000*25);
/*
	   dtt = LPC_TIM0->TC - t0;
	   if( dtt < dt0*1000*25)
	   {  dt =  dt0*1000*25 - dtt;
	     // dt =  dt0*1000*25;
	      if(dt > StolSim.TimerKoeff)
		  {  n = dt / (int)StolSim.TimerKoeff;
//		     if(IRQen[TIMER2_IRQn])
			 {    for(i=0; i< n; i++)
			      {  StolSim.IncrTimer();
			         StolSim.IncrTimer2();
			      }
			 } 
//		  else {
//				  StolSim.IncrTimer(n);
//			 }
	         dtt = LPC_TIM0->TC - t0;
			 dt =  dt0*1000*25 - dtt;
			 LPC_TIM0->TC += dt;
		  }
	   }
*/
       t0 = LPC_TIM0->TC;
       StolSim.WatchWork();
//LPC_TIM2
#if 0
       t = clock();
	   StolSim.tclock = t;
	   dt0 = t - tclock20;
	   if(dt0  > 500) dt0 = 500;  // если у нас были остановки в отладчике
	   tclock20 = t;
	   dtt2 = LPC_TIM2->TC - t20;
	   if( dtt2 < dt0*1000*25)
	   {  dt =  dt0*1000*25 - dtt2;
	      if(dt > StolSim.TimerKoeff)
		  {  n = dt / (int)StolSim.TimerKoeff;
		     for(i=0; i< n; i++)
			 {  StolSim.IncrTimer2();
			 }
//	         dtt = LPC_TIM2->TC - t20;
//			 dt =  dt0*1000*25 - dtt;
//			 LPC_TIM2->TC += dt;
		  }
	   }
       t20 = LPC_TIM2->TC;
#endif //0
//Temperature
	   StolSim.Temperature();
   }
}

int Stol_Sim::Temperature(void)
{
	static int  tclockOld2 = 0, onold=0;
	static double a = 2*1./240., F = 2*320./240., Thead0=0.;
	double dt;
	double T, T1;
	int heaton = 0;

	if (tTempclockOld == tclock)
		return 0;
	dt = (tclock - tTempclockOld) * 0.001;
	if (dt > 1)   // чтоб не было скачков при отладке
		dt = 1.;

#if MOTHERBOARD == BOARD_RAMPS_14_LPC1768_LS_MINI
extern int TMP03_idT0[2], TMP03_idT2[2];
extern float TMP03_CoeffSensorA[2];
extern float TMP03_CoeffSensorB[2];
float celsius[2]={ 22.0f, 25.f};
#if HEATER_0_PIN_EXIST

	if(tTempclockOld == tclock)
		  return 0;
	dt = (tclock - tTempclockOld) * 0.001;
	if(dt > 1)   // чтоб не было скачков при отладке
		dt = 1.;
	heaton = _read(HEATER_0_PIN);

    TempImitStep(heaton,  dt, T, T1);
	tTempclockOld = tclock;
	 celsius[0] = T;
	 celsius[1] = T1;
#endif
	TMP03_idT0[1] = 50000;
    TMP03_idT0[0] =	(celsius[0] - TMP03_CoeffSensorA[0])/TMP03_CoeffSensorB[0] * float(TMP03_idT0[1]);
	TMP03_idT2[1] = 50000;
    TMP03_idT2[0] =	(celsius[1] - TMP03_CoeffSensorA[0])/TMP03_CoeffSensorB[0] * float(TMP03_idT2[1]);

#elif  MOTHERBOARD == BOARD_RAMPS_14_LPC1768

#if	HEATER_BED_PIN_EXIST

	heaton = _read(HEATER_BED_PIN);
	TempImitStep1(heaton, 0, dt, T);
 #if ENABLED(BED_USES_THERMISTOR)
  {
		float celsius = T;
		int raw = 1;
		byte i;
// const short  BEDTEMPTABLE - попугаи, температура
		if (celsius >= BEDTEMPTABLE[0][1])
		{	raw = BEDTEMPTABLE[0][0];	// Overflow: Set to first value in the table
		} else {
			for (i = 1; i < BEDTEMPTABLE_LEN; i++) {
				if ((BEDTEMPTABLE[i][1]) < celsius) {
					raw  = (BEDTEMPTABLE[i - 1][0]) +
						(celsius - (BEDTEMPTABLE[i - 1][1])) *
						(float)((BEDTEMPTABLE[i][0]) - (BEDTEMPTABLE[i - 1][0])) /
						(float)((BEDTEMPTABLE[i][1]) - (BEDTEMPTABLE[i - 1][1]));
					break;
				}
			}
		}
		// Anderflow: Set to last value in the table

		if (i == BEDTEMPTABLE_LEN) raw = (BEDTEMPTABLE[i - 1][0]);

		LPC_ADC->DR[TEMP_BED_PIN] = (raw << 2);
  }
 #endif
#endif //HEATER_BED_PIN_EXIST

#if	HEATER_0_PIN_EXIST
	heaton = _read(HEATER_0_PIN);
	TempImitStep1(heaton, 1, dt, T);
	
 #if ENABLED(HEATER_0_USES_THERMISTOR)
  {
		float celsius = T;
	int raw = 1;
	byte i;
	// const short  BEDTEMPTABLE - попугаи, температура
	if (celsius >= HEATER_0_TEMPTABLE[0][1])
	{
		raw = HEATER_0_TEMPTABLE[0][0];	// Overflow: Set to first value in the table
	}
	else {
		for (i = 1; i <  HEATER_0_TEMPTABLE_LEN; i++) {
			if ((HEATER_0_TEMPTABLE[i][1]) < celsius) {
				raw = (HEATER_0_TEMPTABLE[i - 1][0]) +
					(celsius - (HEATER_0_TEMPTABLE[i - 1][1])) *
					(float)((HEATER_0_TEMPTABLE[i][0]) - (HEATER_0_TEMPTABLE[i - 1][0])) /
					(float)((HEATER_0_TEMPTABLE[i][1]) - (HEATER_0_TEMPTABLE[i - 1][1]));
				break;
			}
		}
	}
	// Anderflow: Set to last value in the table

	if (i == HEATER_0_TEMPTABLE_LEN) raw = (HEATER_0_TEMPTABLE[i - 1][0]);

	LPC_ADC->DR[TEMP_0_PIN] = (raw << 2);
  }
 #endif
#endif //HEATER_0_PIN_EXIST

  //	thermalManager.current_temperature_bed = T;
	tTempclockOld = tclock;

#endif //MOTHERBOARD

#if 0
	if(tclockOld == tclock)
		  return 0;
	 if((tclock - tclockOld2)%1000 == 0)
		  printf("Thead %f %i\n", Thead, onold);
	dt = (tclock - tclockOld2) * 0.001;
/* нагрев */
	 if(!(LPC_GPIO2 ->FIOPIN & 0x010))
	 {  if(onold == 0)
		{  onold = 1;
		   tclockOld2 = tclock;
           dt = 0;
		   T = Thead0 = Thead;
		} else {
	       T = F/a + (Thead0 - F/a)* exp(-a * dt);
	    }
	 } else { //охлаждение
        if(onold == 1)
		{  onold = 0;
		   tclockOld2 = tclock;
           dt = 0;
		   T = Thead0 = Thead;
		} else {
	       T = Thead0 * exp(-a * dt);
	    }
	 }
//	 Thead = (T + Thead) * 0.5;
	 Thead = T;

	 tclockOld = tclock;
#endif //0
	 return 0;
}

/* имитаци€ значени€ напр€жени€ с датчика температуры  */
int GetSSPimit(void)
{  int v =0;
#if 0
   if(StolSim.Thead < 24) v = 950;
   else if(StolSim.Thead < 100)
   {   v = 949 + (StolSim.Thead-24) * (930-949)/ (100-24);
   } else if(StolSim.Thead < 196) {
       v = 930 + (StolSim.Thead-100) * (450-930)/ (196-100);
   } else if(StolSim.Thead < 305) {
	   v = 949 + (StolSim.Thead-24) * (100-949)/ (305-24);
   } else {
	   v = 100 - (StolSim.Thead - 305);
   }
#endif //0
   return v;
}

/******************************************************************/
/* timers */
int SysTick_Config(unsigned int sclkperiod)
{
extern volatile uint32_t _millis; //  millisecond global counter
//	StolSim.SysTickFreq = freq;
    StolSim.SysTickPeriod = sclkperiod/4; /* 100Mhz /4 */
	_millis = 0;
   return 0;
}

void NVIC_EnableIRQ(int irqn)
{  if(irqn >= 0 && irqn <10) 
	   IRQen[irqn] = 1;
}
void NVIC_DisableIRQ(int irqn)
{  if(irqn >= 0 && irqn <10) 
	   IRQen[irqn] = 0;
}
void NVIC_SetPendingIRQ(int irqn)
{  
	      TIMER2_IRQHandler();
		  if(LPC_TIM2->TCR !=  0x01) // финт ушами - по LPC_TIM2->TCR=  0x02;  reset & stop counter 
			  LPC_TIM2->TC = 0;
}

void NVIC_SetPriority(int irqn, int a)
{
}

int NVIC_EncodePriority(int a,int b,int c)
{   return 0;
}

/* _TIMER2_IRQHandler already defined in stepper.cpp
void TIMER2_IRQHandler(void)
{
  printf("todo"__FUNCTION__"\n");
}
*/
/* работа след€щей системы */
int Stol_Sim::WatchWork(void)
{   static int  sts = 0;
    static int t0=0;
#if 0
//входные сигналы
    switch(sts)
    {   case 0: // начальное состо€ние
            if(LPC_GPIO2 ->FIOPIN & 0x8)
            {   t0 = clock();
                sts = 1;
            } 
       break;
        case 1: //переход в  точку вреза
         if(clock()-t0 > 500)
         {   sts = 2;
         }  
         if(!(LPC_GPIO2 ->FIOPIN & 0x8))
         {   sts = 20;
         }
       break;
        case 2: //
          outSE  = 1;
            if(LPC_GPIO2 ->FIOPIN & 0x1)
            {   t0 = clock();
                sts = 3;
            }          
       break;
        case 3: //переход в  точку реза
         if(clock()-t0 > 500)
         {   sts = 2;
         }  
         if(!(LPC_GPIO2 ->FIOPIN & 0x8) || !(LPC_GPIO2 ->FIOPIN & 0x1))
         {   sts = 20;
         }
       break;
        case 4: //
          outZR  = 1;
         if(!(LPC_GPIO2 ->FIOPIN & 0x8) || !(LPC_GPIO2 ->FIOPIN & 0x1))
         {   sts = 20;
         }
       break;
        case 20: //прекращение слежени€
            {   t0 = clock();
                sts = 21;
            }          
       break;
        case 21: //
         if(clock()-t0 > 10)
         {   sts = 22;
             outZR = 0;
             outSE = 0;
             t0 = clock();
         }  

        case 22: //прекращение слежени€
         if(clock()-t0 > 50)
            {   
                sts = 0;
            }          
       break;
    }
#endif //0
   return 0;
}

int Stol_Sim::IncrTimer(int n)
{  unsigned int t01;
   int i, dp, dt;
   t01 = LPC_TIM1->TC + ((unsigned int)TimerKoeff);
   if(t01 <LPC_TIM1->TC)
   {  for(i=0; i<n; i++) IncrTimer();
      return 0;
   }
#if 0
   if(t01 - IncrTimer_t0 > 128)
   {	dt = (t01 - IncrTimer_t0)/128;
		for(i = 0; i<4; i++)
		{	if(slave[i].XposTTLtag != slave[i].Xpos)
			{	dp = slave[i].XposTTLtag - slave[i].Xpos;
				if(dp > dt)
				  dp = dt;
				else if(dp < -dt)
					dp = -dt;
				slave[i].Xpos += dp;
				x[i] = slave[i].Xpos * Mashtab[i];
			}
		}
	
      IncrTimer_t0 = t01;
   }
#endif //0
    return 0;
}

//Keil: Timer0 is used for TCP retransmission control
//emulator: Timer0 25Mhz
int Stol_Sim::IncrTimer(void)
{   int i, dp, dt;
    int n;
	unsigned int udt;
//Traz++;
//if(Traz > 2)
//{  Tms = (double)(clock()-Tt0) / double(Traz);
//}

	LPC_TIM0->TC += ((unsigned int)TimerKoeff);
	dt = LPC_TIM0->TC;
// if(dt <0)
//			printf("херь0");

	while(int(LPC_TIM0->TC-SysTickClockT)  >=  (int)SysTickPeriod)
	{   dt = LPC_TIM0->TC - SysTickClockT;
	    if(dt <= 0)
			printf("херь");
		udt = LPC_TIM0->TC - SysTickClockT;

	    n = dt/ SysTickPeriod;
		if(n > 1000)
			printf("херь");
		SysTick_Handler();
		SysTickClockT += SysTickPeriod;
	}
#if 0	
 if(LPC_TIM1->TC - IncrTimer_t0 > 128)
 {  dt = (LPC_TIM1->TC - IncrTimer_t0)/128;
	for(i = 0; i<4; i++)
	{  if(slave[i].XposTTLtag != slave[i].Xpos)
	   {  dp = slave[i].XposTTLtag - slave[i].Xpos;
	      if(dp > dt)
			  dp = dt;
		  else if(dp < -dt)
			   dp = -dt;
		  slave[i].Xpos += dp;
	      x[i] = slave[i].Xpos * Mashtab[i];
	   }
	}
    IncrTimer_t0 = LPC_TIM1->TC;
 }
#endif //0
	return 0;
}
static int raz = 0;
static int delta_t0[1000];
static int delta_t[1000];
static int testBreak = 0;
extern 	"C" int  irqs_is_enabled;

int Stol_Sim::IncrTimer3(unsigned int  ad_t)
{   int dt, dt0, dt1, dt2, dts, n=0;
    unsigned int t0, t1, t2, ts;
	int tmpDtSysTick = 0;

	if (testBreak)
		printf("testBreak");
M0:
	t0 = LPC_TIM0->TC + ad_t;
//calc next time interrupt time for TIM1 & TIM2
//	dt1 = LPC_TIM1->MR0 - LPC_TIM1->TC;
	dt1 = LPC_TIM1->MR0;
	if(LPC_TIM1->PR)
	{  dt1 *= (LPC_TIM1->PR+1);
	} 
	t1 = LPC_TIM1->RESERVED0[0] + dt1;

//	dt2 = LPC_TIM2->MR0 - LPC_TIM2->TC;
	dt2 =  LPC_TIM2->MR0;
	if(LPC_TIM2->PR)
	{  dt2 *= (LPC_TIM2->PR+1);
	} 
	t2 = LPC_TIM2->RESERVED0[0] + dt2;
	ts = SysTickClockT + SysTickPeriod;
	/*------------------------ */
	dt1 = t1 - LPC_TIM0->TC;
	dt2 = t2 - LPC_TIM0->TC;
	dts = ts - LPC_TIM0->TC;
	dt0 = dt1;
	if(dt2 < dt1) dt0 = dt2;
	if(dts < dt0) dt0 = dts;

	if (ad_t < dt0)
	{
		LPC_TIM0->TC = t0;
		return n;
	}

	LPC_TIM0->TC += ((unsigned int)dt0);

	if(int(LPC_TIM0->TC - SysTickClockT)  >=  (int)SysTickPeriod)
	{  SysTick_Handler();
	tmpDtSysTick++;
	if (raz < 1000)
	{
		if (raz == 0)
		{
			delta_t0[0] = LPC_TIM0->TC;
			delta_t[0] = 0;
		} else {
			delta_t0[raz] = LPC_TIM0->TC;
			delta_t[raz] = LPC_TIM0->TC - delta_t0[raz - 1];
			if (delta_t[raz] > 100000)
				testBreak = 1;
		}
			raz++;
	}	else {
//		printf("стоп\n");
	}

	   SysTickClockT += SysTickPeriod;
	}

    dt = LPC_TIM0->TC -	LPC_TIM1->RESERVED0[0];
	if(LPC_TIM1->PR)
	{    LPC_TIM1->TC += dt/(LPC_TIM1->PR+1);
	} else LPC_TIM1->TC += dt;
	if(LPC_TIM1->TC >= LPC_TIM1->MR0)
	{  LPC_TIM1->RESERVED0[0] = LPC_TIM0->TC;
	  
		if(IRQen[TIMER1_IRQn] && irqs_is_enabled)
		{     TIMER1_IRQHandler();
		}
	    LPC_TIM1->RESERVED0[0] = LPC_TIM0->TC;
		LPC_TIM1->TC = 0;
	}  

    dt = LPC_TIM0->TC -	LPC_TIM2->RESERVED0[0];
	if(LPC_TIM2->PR)
	{    LPC_TIM2->TC += dt/(LPC_TIM2->PR+1);
	} else LPC_TIM2->TC += dt;
	if(LPC_TIM2->TC >= LPC_TIM2->MR0)
	{  LPC_TIM2->RESERVED0[0] = LPC_TIM0->TC;
	  	if(IRQen[TIMER2_IRQn] && irqs_is_enabled)
		{     TIMER2_IRQHandler();
		}
	    LPC_TIM2->RESERVED0[0] = LPC_TIM0->TC;
		LPC_TIM2->TC = 0;
	}  
	if(ad_t > dt0)
	{   ad_t -= dt0;
	    n++; 
	    goto M0;
	}
	return n;
}

int Stol_Sim::IncrTimer2(void)
{   int i, dp, dt, dt1;
static unsigned int t0;
    dt = LPC_TIM0->TC -	LPC_TIM1->RESERVED0[0];
	if(LPC_TIM1->PR)
	{    LPC_TIM1->TC += dt/(LPC_TIM1->PR+1);
	} else LPC_TIM1->TC += dt;
	if(LPC_TIM1->TC >= LPC_TIM1->MR0)
	{  LPC_TIM1->RESERVED0[0] = LPC_TIM0->TC;
	  	if(IRQen[TIMER1_IRQn])
		{     TIMER1_IRQHandler();
		}
	    LPC_TIM1->RESERVED0[0] = LPC_TIM0->TC;
		LPC_TIM1->TC = 0;
	}  

    dt = LPC_TIM0->TC -	LPC_TIM2->RESERVED0[0];
	if(LPC_TIM2->PR)
	{    LPC_TIM2->TC += dt/(LPC_TIM2->PR+1);
	} else LPC_TIM2->TC += dt;
	if(LPC_TIM2->TC >= LPC_TIM2->MR0)
	{  LPC_TIM2->RESERVED0[0] = LPC_TIM0->TC;
	  	if(IRQen[TIMER2_IRQn])
		{     TIMER2_IRQHandler();
		}
	    LPC_TIM2->RESERVED0[0] = LPC_TIM0->TC;
		LPC_TIM2->TC = 0;
	}  
    return 0;
}

int Stol_Sim::setActiveController(int nslave, int OnOff)
{	
#if 0
	if(slave[nslave].isAct == 0 && OnOff)
	{   char buf[16];
	    *((int *)&buf[0]) = slave[nslave].Xpos;
	    buf[4] = (slave[nslave].Kon0<<4)|slave[nslave].KonAK;
		  WriteMessageFromSlave(nslave, buf, 5);
	}
	slave[nslave].isAct = OnOff;
#endif //0
	return 0;
}

int Stol_Sim::SetU(int nslave, short int U)
{   int cTrlSum;
#if 0
	slave[nslave].Uout = U;
    slave[nslave].nOut = 3;
	cTrlSum = 0;
	slave[nslave].OutBuff[0] = SETU;
	cTrlSum += slave[nslave].OutBuff[0];
	slave[nslave].OutBuff[1] = slave[nslave].errXpos;
	cTrlSum += slave[nslave].OutBuff[1];
	slave[nslave].OutBuff[2] = cTrlSum;
#endif //0
    return 0;
}

int Stol_Sim::GetPar(int nslave)
{   int  i, l;
    char buff[256];    
#if 0
    lpc0.Xpos = slave[nslave].Xpos;
    lpc0.Kon0 = slave[nslave].Kon0; 
    lpc0.Kon1 = slave[nslave].Kon1;
    lpc0.KonAK = slave[nslave].KonAK;
	lpc0.Uout = slave[nslave].Uout;
	l = sizeof(struct LPC_Slave);
	memcpy(&buff[1], &lpc0, l);
	buff[0] = GETPAR;
    WriteMessageFromSlave(nslave, buff, l+1);
#endif  //0
    return 0;
}

int Stol_Sim::WriteMessageFromSlave(int nslave, char *buf, int l)
{   int i,crs;
#if 0
    crs = 0;
    for(i=0; i<l; i++)
    {  crs += buf[i];
	   slave[nslave].OutBuff[i] = buf[i];
	}
	slave[nslave].OutBuff[l] = crs;
    slave[nslave].nOut = l+1;
#endif //0
	return 0;
}

  int Stol_Sim::SetSlaveKonOut(int nslave, int KonOut)
  {  int cTrlSum;
#if 0
	  slave[nslave].KonOut = KonOut;
     
    slave[nslave].nOut = 3;
	cTrlSum = 0;
	slave[nslave].OutBuff[0] = SETKON;
	cTrlSum += slave[nslave].OutBuff[0];
	slave[nslave].OutBuff[1] = 0;
	cTrlSum += slave[nslave].OutBuff[1];
	slave[nslave].OutBuff[2] = cTrlSum;
#endif 
	return 0;
  }

 
