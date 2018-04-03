/* Master_4.cpp */
/* низкоуровневые функции из master.c которые должны иметь доступ к классу stol_4 */
#ifndef WIN32
#include <Net_Config.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "LPC17xx.h"		// Keil: Register definition file for LPC17xx
#ifndef WIN32
#include <RTL.h>
#endif

#include "Master.h"
#include "config_0.h"
#include "mbed.h"
#include "HAL/HAL.h"
#include "inc/MarlinConfig.h"
#include "mybuffer.hpp"
#include "planner.h"
#include "stepper.h"


/*********************************/
/* external function prototypes  */
/*********************************/
extern int SetKon(int kon);
extern "C" void * GetCfgPtrFromFlash(void);
extern "C" int WriteCfg_To_Flash(void *pconfig, int len);
extern void Led1On(int onOff);
extern void on_get_tcp(char *buf, int len);

/*********************************/
/* internal function prototypes  */
/*********************************/
extern "C" U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len);
extern "C" void UdpSend(void);

extern "C" int ReadConfigFromFlash(void);
extern "C" int  WriteConfigToFlash(void);

void WriteConfigToFlashInit(void);

int udp_callback(void);
void udp_callback_test(U16 len, short int cmd, unsigned short int par,  U8 *bf, int Nlost);
void udp_callback_Default(short int cmd,  U8 *bf);
void udp_callback_HandShake( U8 *bf);
void udp_callback_Identify( U8 *bf);
void udp_callback_Echo( U16 len, U8 *bf);
void udp_callback_getlog( U8 *bf);
void udp_callback_GetInfo( U8 *bf);
void udp_callback_Getpar( U8 *bf);
void udp_callback_Gcmd( U8 *bf);
void udp_callback_SetOut( U8 *bf);


const char  BiosDate[12] = __DATE__;     /* дата компиляции биоса */

  const int BiosCode=3;     /* код биоса: 1 - привода */
  const int Vers=0;         /* версия */
  const int SubVers=0;      /* подверсия */

extern LPC1768Serial customizedSerial;

/*--------------------------- ----------------------------------*/
U8 socket_udp;
U8 socket_udp1;

//unsigned char  UdpSendDataBuffer[512];
U8 UdpRemIp[4];
U16 UdpRport=0;
int UdpLsend=0;
int UdpOn = 0;	
PACKED unsigned char *UdpMsgOut=NULL;

#ifndef WIN32

void UdpSend(void)
{  if(UdpLsend)
   {  udp_send (socket_udp, UdpRemIp, UdpRport, UdpMsgOut, UdpLsend);
      UdpLsend = 0;
//Led1On(0);
		 if(UdpOn > 1) UdpOn--;
   }
}
#else
extern unsigned char * udp_get_buf(int Nsend);
#endif //

/*--------------------------- Udp send --------------------------------------*/
void send_udp(char *buf, int len)
{

	if(len > 1460-6) 
		len = 1460-6;
   UdpMsgOut = udp_get_buf (len+6);
	
	   if(UdpMsgOut)
		 {
			*((PACKED short int *) (&UdpMsgOut[0])) = 0x4000; 
			*((PACKED short int *) (&UdpMsgOut[2])) = MCMD_MSG; /*  cmd */
			*((PACKED short int *) (&UdpMsgOut[4])) = len; /* */
			 memcpy(&UdpMsgOut[6], buf, len);
				UdpLsend = len+6;
			 net_stuff();
		 }
}

extern U16 UdpRport; //=0;
extern int UdpLsend; //=0;
// int jopa3=0, jopa4=0, jopa5=0, jopa6=0;

U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *bf, U16 len) 
{  short int cmd;
   unsigned short int par;
   int  isrep=0;
static unsigned short lastInd=0;
static unsigned int  Nlost=0;
//Led1On(1);
UdpOn++;
	
  memcpy((void *)&UdpRemIp[0],(void *)rip,4);
  UdpRport = rport;
  if (soc != socket_udp) 
  { if (soc == socket_udp1) 
  	{  UdpLsend = 8;
	   UdpMsgOut = udp_get_buf (UdpLsend);
	   memcpy((void *)&UdpMsgOut[0],(void *)bf,6);
	   UdpMsgOut[6]='O';
	   UdpMsgOut[7]='K';
  	}
    return (0);
  }	
  cmd  = *((short int *)&bf[2]);
  par  = *((unsigned short int *)&bf[4]);
  if(par != (lastInd+1)) /* параметр всегда должен инкрементироваться, чтобы обеспечить защиту от повторных посылок */
  {  if(par == lastInd)
             isrep = 1; /* повторная посылка */
	 else 
	         Nlost++;                 /* потеря данных */  
  }
  lastInd = par;
  if(cmd & 0x8000) //тест обмена
  {    udp_callback_test(len, cmd, par, bf, Nlost);
	   return 0;
  }	
   switch(cmd)
   {  

		 case MCMD_HAND_SHAKE:
             udp_callback_HandShake(bf);
	        break;
		 case MCMD_ECHO:
             udp_callback_Echo(len, bf);
	        break;
		 case MCMD_IDENTIFY:
             udp_callback_Identify(bf);
		break;
		 case MCMD_GETLOG:
             udp_callback_getlog(bf);
			 break;

  	  case MCMD_SETKON: 
             udp_callback_SetOut(bf);
	        break;
		case MCMD_GETINFO:
             udp_callback_GetInfo(bf);
		
	        break;
		case MCMD_GCMD_M:
           udp_callback_Gcmd(bf);
	        break;
		case MCMD_GETPAR:
           udp_callback_Getpar(bf);
	        break;

#if 0
	  case MCMD_GETINFO_ZEROS: /* выдать информацию о процессе поиска нуля */
             stol.udp_callback_GetInfoZeros(bf);
	        break;
	  case MCMD_SET_STS_HAND:
             stol.udp_callback_SetStsHand(bf);
	        break;
	  case MCMD_SET_STS_HAND3:
             stol.udp_callback_SetStsHand3(bf);
	        break;
	  case MCMD_SET_STS_HAND4:
             stol.udp_callback_SetStsHand4(bf);
	        break;
		
	  case MCMD_SETDRIVEREADY: /* input: 1 int, output: 1 int*/
             stol.udp_callback_SetDriveReady(bf);
	        break;
	  case MCMD_SETSTROB: 
             stol.udp_callback_SetStrob(bf);
	        break;
   	 case MCMD_ADD_BUFF_XY:
             stol.udp_callback_Add_Buff(bf, isrep);
	        break;	  
	 case MCMD_ADD_BUFF_XY2:
             stol.udp_callback_Add_Buff2(bf, isrep);
		    break;	  
 	 case MCMD_GETLOG: /* запрос лога */
             stol.udp_callback_GetLog(bf);
	        break;
	 case MCMD_CLEAR_BUFF:
             stol.udp_callback_ClearBuff(bf);
	        break;
	 case MCMD_FINDZERO: /* искать ноль */
             stol.udp_callback_FindZero(bf);
	        break;
	 case MCMD_SET_SHIFT:
             stol.udp_callback_SetShift(bf, isrep);
	        break;
	 case MCMD_GETDRIVE_PAR:  /* запрос параметров привода */
             stol.udp_callback_GetDrivePar(bf);
	        break;
	 case MCMD_SETDRIVE_TAG: /* задать целевое положение привода (в основном - для смещения Z)*/
             stol.udp_callback_SetDriveTag(bf);
	        break;
	  case MCMD_GET_TEMP: /* запрос температуры */
             stol.udp_callback_GetTemp(bf);
	        break;
	  case MCMD_SET_TEMP: /* Задать температуру и параметры: 0/1 вкл/выкл - граница включения - граница выключения */
             stol.udp_callback_SetTemp(bf);
	        break;
	 case MCMD_SET_TEST: /* установить режим тестирования */
             stol.udp_callback_SetTest(bf);
	        break;
     case MCMD_SET_BUFF_LEN: //задать размеры буфера
             stol.udp_callback_SetBuffLen(bf);
	        break;
#endif		 
	 default:
             udp_callback_Default(cmd, bf);
   }  
/*
	 if(stol.sts_next == LPC_HAND)
				   jopa3++;
		  if(stol.sts == LPC_HAND)
				   jopa4++;
		  if(stol.sts == LPC_MOVEKVANT_XY2)
				   jopa5++;
		  if(stol.sts_next == LPC_MOVEKVANT_XY2)
				   jopa6++;
*/	 
  return (0);				   
}


//Default error 
void udp_callback_Default(short int cmd,  U8 *bf)
 {  UdpLsend = 6+sizeof(int)+sizeof(short int);
    UdpMsgOut = udp_get_buf(UdpLsend);
	memcpy((void *)&UdpMsgOut[0],(void *)&bf[0],6);
	*((PACKED short int *) (&UdpMsgOut[0])) |= 0x8000; /* error: wrong cmd */
	*((PACKED int *) (&UdpMsgOut[6])) = cmd; /* wrong cmd */
}


// MCMD_HAND_SHAKE
void udp_callback_HandShake( U8 *bf)
{ int l;
  l = sizeof(HAND_SHAKE_OUT);
	UdpLsend = l + sizeof(short int)*3+1; //17
	UdpMsgOut = udp_get_buf(UdpLsend);
	memcpy((void *)&UdpMsgOut[0],(void *)bf,6);
	if((bf[6+l-1] == 0) && !strcmp((char *)&bf[6], HAND_SHAKE_INP))
	{   memcpy(&UdpMsgOut[6], HAND_SHAKE_OUT,l);
	} else {
	    memcpy(&UdpMsgOut[6], HAND_SHAKE_ERR,l);
	}		 	
}


//MCMD_ECHO:
void udp_callback_Echo( U16 len, U8 *bf)
{		UdpLsend = len; 
		UdpMsgOut = udp_get_buf(UdpLsend);
		memcpy((void *)&UdpMsgOut[0],(void *)bf, UdpLsend);
}

//MCMD_IDENTIFY
void udp_callback_Identify( U8 *bf)
{ int l; 
	
	l = sizeof(IDENTIFY_TEXT);
  UdpLsend = 6+sizeof(int)*2+sizeof(short int)+l;	
  UdpMsgOut = udp_get_buf(UdpLsend);
	memcpy((void *)&UdpMsgOut[0],(void *)bf,6);
  *((PACKED short int *) (&UdpMsgOut[6])) = (short int)(sizeof(int)*2+l); 
  *((PACKED  int *) (&UdpMsgOut[8]))  =  IDENTIFY_CODE;
  *((PACKED  int *) (&UdpMsgOut[12]))  = 0; // IdNumber;	
	memcpy((void *)&UdpMsgOut[16],(void *)IDENTIFY_TEXT, l);
}

extern myBuffer2 debugBuff;

void udp_callback_getlog( U8 *bf)
{ int i, l, lm, rc; 
	unsigned int data[2];
	PACKED  int *ptr;
	lm = *((PACKED short int *) (&bf[6]));
	l = debugBuff.GetLbuf(); //length in elements
	if(l > lm) l = lm;
  UdpLsend = 6+sizeof(short int) + sizeof(int)*(l*2);	
  UdpMsgOut = udp_get_buf(UdpLsend);
	memcpy((void *)&UdpMsgOut[0],(void *)bf,6);
  *((PACKED  short int *) (&UdpMsgOut[6])) = (short int)(l); 
	ptr = (PACKED  int *)&UdpMsgOut[8];
	for(i=0; i<l; i++)
	{ rc = debugBuff.Get(data);
		if(rc) break;
		*ptr++ = data[0];
		*ptr++ = data[1];
	}
	
}

int tmp_flag = 0;
//MCMD_GETINFO
void udp_callback_GetInfo( U8 *bf)
{   int i, v, sts;
extern	float current_position[4];
#if defined(USE_TMP03)
extern	int TMP03_idT0[2], TMP03_idT2[2];
#endif
extern	float ADC_values[8];
extern bool axis_homed[XYZ], axis_known_position[XYZ];
extern Stepper stepper;
	
	
      UdpLsend = 106;
      UdpMsgOut = udp_get_buf(UdpLsend);
	   memcpy((void *)&UdpMsgOut[0],(void *)bf,6);
//todo	
	   memset((void *)&UdpMsgOut[6],0,100);
	
	sts = 0;
	if(axis_known_position[0]) sts |= 1;
	if(axis_known_position[1]) sts |= 2;
	*((PACKED short int *) (&UdpMsgOut[6])) = sts;
tmp_flag = 1;	
  v = 0;
	if(READ(X_MIN_PIN)^X_MIN_ENDSTOP_INVERTING) v |= 0x01;
	if(READ(X_MAX_PIN)^X_MAX_ENDSTOP_INVERTING) v |= 0x02;
	if(READ(Y_MIN_PIN)^Y_MIN_ENDSTOP_INVERTING) v |= 0x04;
	if(READ(Y_MAX_PIN)^Y_MAX_ENDSTOP_INVERTING) v |= 0x08;
	*((PACKED int *) (&UdpMsgOut[8])) = v;
tmp_flag = 0;	
	
	memcpy((void *)&UdpMsgOut[16],(void *)&current_position,16);// xTag в мм
#if defined(USE_TMP03)
	memcpy((void *)&UdpMsgOut[32],(void *)&TMP03_idT0,8);
	memcpy((void *)&UdpMsgOut[40],(void *)&TMP03_idT2,8);
#endif
	memcpy((void *)&UdpMsgOut[48],(void *)&ADC_values,8); //0,1 ADC chan
	
	//memcpy((void *)&UdpMsgOut[56],(void *)&stepper.count_position,16); 
	*((PACKED int *) (&UdpMsgOut[56])) = stepper.position(X_AXIS); //реальная позиция в шагах
	*((PACKED int *) (&UdpMsgOut[60])) = stepper.position(Y_AXIS); //реальная позиция в шагах
	*((PACKED int *) (&UdpMsgOut[64])) = stepper.position(Z_AXIS); //реальная позиция в шагах
	

	#if 0
	
	*((PACKED short int *) (&UdpMsgOut[6])) = sts;
	*((PACKED int *) (&UdpMsgOut[8])) = KonIn;
	*((PACKED int *) (&UdpMsgOut[12])) = KonOut;
	memcpy((void *)&UdpMsgOut[16],(void *)&privod[0].Xpos,8);
	memcpy((void *)&UdpMsgOut[24],(void *)&privod[1].Xpos,8);
	memcpy((void *)&UdpMsgOut[32],(void *)&privod[2].Xpos,8);
	memcpy((void *)&UdpMsgOut[40],(void *)&privod[3].Xpos,8);
	*((PACKED int *) (&UdpMsgOut[48])) = slave_sts;
	UdpMsgOut[52] = privod[0].kon;
	UdpMsgOut[53] = privod[1].kon;
	UdpMsgOut[54] = privod[2].kon;
	UdpMsgOut[55] = privod[3].kon;
	*((PACKED float *) (&UdpMsgOut[56])) = v[0];
	*((PACKED float *) (&UdpMsgOut[60])) = v[1];
	*((PACKED int *) (&UdpMsgOut[64])) = privod[indW].vXtag ; //timeKvant;
       
    i = WorkBuffer.GetLbuf(); /* Сколько занято в int в рабочем буфере */
	*((PACKED short int *) (&UdpMsgOut[68])) = (short int)i; 
    i = LogBuffer.GetLbuf(); /* Сколько занято в int в буфере лога */
	*((PACKED short int *) (&UdpMsgOut[70])) = (short int)i; 

	*((PACKED short int *) (&UdpMsgOut[72])) = (short int)lpc[0].errXpos;
	*((PACKED short int *) (&UdpMsgOut[74])) = (short int)lpc[1].errXpos;
	*((PACKED short int *) (&UdpMsgOut[76])) = (short int)lpc[2].errXpos;
	*((PACKED short int *) (&UdpMsgOut[78])) = (short int)lpc[3].errXpos;
	*((PACKED float *) (&UdpMsgOut[80])) = v[2]; //Vz
	*((PACKED float *) (&UdpMsgOut[84])) = v[3]; //Vw
	*((PACKED float *) (&UdpMsgOut[88])) = SSP_ADC[0];
	*((PACKED float *) (&UdpMsgOut[92])) = SSP_ADC[1];
	*((PACKED short int *) (&UdpMsgOut[96])) = (short int)ErrorCode;
	*((PACKED int *) (&UdpMsgOut[98])) = fkv.kadr;
	*((PACKED int *) (&UdpMsgOut[102])) = 0; // W
#endif //0	
  i = debugBuff.GetLbuf(); /* Длина занятого буфера в элементах */
	*((PACKED short int *) (&UdpMsgOut[70])) = (short int)i; 

}

//MCMD_GCMD_M
void udp_callback_Getpar( U8 *bf)
{   int i;
extern Planner planner;
	
      UdpLsend = 106;
      UdpMsgOut = udp_get_buf(UdpLsend);
	   memcpy((void *)&UdpMsgOut[0],(void *)bf,6);

	memcpy((void *)&UdpMsgOut[6],(void *)&planner.axis_steps_per_mm,16); //4*sizeof(float)
	
//todo	
//  memset((void *)&UdpMsgOut[26],0,100);

}

//MCMD_GCMD_M
void udp_callback_Gcmd( U8 *bf)
{  int l;
	 UdpLsend = 6+sizeof(int);
   UdpMsgOut = udp_get_buf(UdpLsend);
 	 memcpy((void *)&UdpMsgOut[0],(void *)&bf[0],6);
	 *((PACKED int *) (&UdpMsgOut[6])) = 0; 
    l = *((PACKED int *)&bf[6]); 

	customizedSerial.bRx.Add((char *)&bf[10],l);	
}

#if MOTHERBOARD == BOARD_RAMPS_14_LPC1768_LS_MINI
extern void HeaterOn(int onOff);
extern void ReadLaserOn(int onOff);
extern void ZatvorOn(int onOff);
#endif

void udp_callback_SetOut( U8 *bf)
{  int val, mask, v;
	 UdpLsend = 6+sizeof(int);
   UdpMsgOut = udp_get_buf(UdpLsend);
 	 memcpy((void *)&UdpMsgOut[0],(void *)&bf[0],6);

		val =  *((PACKED int *)&bf[6]);
		mask =  *((PACKED int *)&bf[10]);
//1 - heater
//2 - readlaser	
#if MOTHERBOARD == BOARD_RAMPS_14_LPC1768_LS_MINI  
	 v = 0;
	  if(mask & 0x01)
		{  if(val & 0x01)
			 {  v |= 0x01;
				  HeaterOn(1);
			 }	else
			 HeaterOn(0);			 
		}
	  if(mask & 0x02)
		{  if(val & 0x02)
			 {  v |= 0x02;
				  ReadLaserOn(1);
			 }	else
			 ReadLaserOn(0);			 
		}
	  if(mask & 0x04)
		{  if(val & 0x04)
			 {  v |= 0x04;
				  ZatvorOn(1);
			 }	else
			 ZatvorOn(0);			 
		}
#endif
		*((PACKED int *) (&UdpMsgOut[6])) = v;
}	

#if 0
//MCMD_SET_BUFF_LEN задать размеры буфера
//todo
void stol_4::udp_callback_SetBuffLen( U8 *bf)
{   int i, litem; 
    UdpLsend = 6+sizeof(int)*2;	
    UdpMsgOut = udp_get_buf(UdpLsend);
    i = *((PACKED int *)&bf[6]); 
	litem = *((PACKED int *)&bf[10]); 
	if(i > LBUFF_SIZE || i == 0) i = LBUFF_SIZE;
	if(i < 16) i = 16;
/*	
	buf.Lbuff = i;
 	memcpy((void *)&UdpMsgOut[0],(void *)&bf[0],6);
	bl2.ptr = (int *) &buf.b[buf.Lbuff];
	bl2.ibuf = bl2.ifree = 0;
	bl2.iserr = 0;
	bl2.Lbuff = (LBUFF_SIZE -  buf.Lbuff) * sizeof(struct Master4_buf_item)/sizeof(int);
	bl2.Litem = litem;
	bl2.Lbuff = (bl2.Lbuff/litem)*litem;
    buf.ibuf = buf.ifree = 0;
    *((PACKED  int *) (&UdpMsgOut[6]))  =   buf.Lbuff;
    *((PACKED  int *) (&UdpMsgOut[10])) =   bl2.Lbuff;
*/	
    *((PACKED  int *) (&UdpMsgOut[6]))  =   0;
    *((PACKED  int *) (&UdpMsgOut[10])) =   0;
}

//MCMD_SET_TEST установить режим тестирования 
//todo
void stol_4::udp_callback_SetTest( U8 *bf)
{  int i, mode, mreset;
/* тест приводов */
//todo	
    UdpLsend = 6;
    UdpMsgOut = udp_get_buf(UdpLsend);
}

#endif
////////////////////////////////////////////////
//(cmd & 0x8000) //тест обмена
void udp_callback_test(U16 len, short int cmd, unsigned short int par,  U8 *bf, int Nlost)
{  static int raz=0;
	const int jj = 0xffff;
	
	UdpLsend = len;
      if(UdpLsend > 1400) UdpLsend = 1400;
      UdpMsgOut = udp_get_buf(UdpLsend);
	memcpy((void *)&UdpMsgOut[0],(void *)bf,6);
	*((PACKED int *) (&UdpMsgOut[6])) = ++raz;
	*((PACKED short int *) (&UdpMsgOut[10])) = BiosCode;
	*((PACKED short int *) (&UdpMsgOut[12])) = Vers;
	*((PACKED short int *) (&UdpMsgOut[14])) = SubVers;
	*((PACKED int *) (&UdpMsgOut[16])) = MYLBUFF_SIZE; 
	*((PACKED int *) (&UdpMsgOut[20])) = 0; //WorkBuffer.Lbuf;
	*((PACKED int *) (&UdpMsgOut[24])) = 0; //WorkBuffer.Litem;
	*((PACKED int *) (&UdpMsgOut[28])) = 0; //LogBuffer.Lbuf;
	*((PACKED int *) (&UdpMsgOut[32])) = Nlost;
	memcpy((void *)&UdpMsgOut[36],(void *)&BiosDate,12);
	*((PACKED int *) (&UdpMsgOut[48])) = 0;
	*((PACKED int *) (&UdpMsgOut[52])) = par;
    UdpMsgOut[56] = 0;
    UdpMsgOut[57] = 0;
    UdpMsgOut[58] = (unsigned char)jj;
    UdpMsgOut[59] = (unsigned char)jj;
    UdpMsgOut[60] = (unsigned char)jj;
    UdpMsgOut[61] = (unsigned char)jj;
    UdpMsgOut[62] = (unsigned char)jj;
    UdpMsgOut[63] = (unsigned char)jj;
    UdpMsgOut[64] = (unsigned char)jj;
    UdpMsgOut[65] = (unsigned char)jj;
}

//затычка
int udp_callback(void)
{
	return 0;
}
