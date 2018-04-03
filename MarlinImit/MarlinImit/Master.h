/* Master.h */
#if defined(WIN32)
	#include "SiM.h"
#else
	#define PACKED __packed /* */
#endif


#ifndef STOL4_CONFIG1
#define STOL4_CONFIG1

#define USE_ZWATCH 0
#define USEZPLUS 0

#define HAND_SHAKE_INP           "TCPiptEsT"
#define HAND_SHAKE_OUT           "ipTCPTeSt"
#define HAND_SHAKE_ERR           "NoOkError"

#define MCMD_HAND_SHAKE  		0x82
#define MCMD_ECHO        		0x81
#define MCMD_IDENTIFY       0x80
/* (1 управление движением)(4 число координат) (02 проект)*/
#define IDENTIFY_CODE        		0x1402
#define IDENTIFY_TEXT        		"Powder"


/* Команды мастеру */
#define  MCMD_SETKON	      0x01 //установить выходные регистры
#define  MCMD_GETINFO       0x02 //запрос информации
#define  MCMD_GETINFO_ZEROS 0x03 //запрос информации о поиске нуля
#define  MCMD_SETDRIVEREADY 0x10 //снять/установить готовность приводов (включить-выключить) 
#define  MCMD_ADD_BUFF_XY   0x12 //добавляет в буфер buf и не меняет или меняет состояние на MODE_MOVE_XY */
#define  MCMD_CLEAR_BUFF    0x13 //очистка буфера
#define  MCMD_SETSTROB		0x14 //строб сброса аварии
#define  MCMD_GETLOG		0x15 //запрос лога
#define  MCMD_GETLOG_TEST	0x16 //запрос лога теста
#define  MCMD_GETDRIVE_PAR	0x17 //запрос параметров привода
#define  MCMD_SETDRIVE_TRAPEZE	0x18 //задать параметры движения по трапеции
#define  MCMD_SETDRIVE_STOP  0x19 //плавно остановиться

#define  MCMD_SET_WATCH     0x1b //задать параметры слежения
#define  MCMD_SET_LASER     0x1c //задать параметры лазера
#define  MCMD_SET_STS_HAND  0x20 //задать режим ручного управления 
#define  MCMD_FINDZERO      0x21 //установить режим поиска нуля X/Y/Z
#define  MCMD_SET_STS_HAND3	0x22 //задать режим ручного управления XYZ
#define  MCMD_SET_SHIFT	  	0x23 //задать сдвиг начала координат
#define  MCMD_SETDRIVE_TAG	0x24 //задать целевое положение привода
#define  MCMD_SET_TEST		  0x30 //задать режим теста
#define  MCMD_SET_ADC_N     0x33 //установить число измерений ADC выдавать сумму по N измерениям+сброс
#define  MCMD_GET_ADC       0x34 //выдать текущие значения  по N измерениям датчиков уровня+тока красного лазера
#define  MCMD_SET_INITED    0x40 //установить признак "инициализация прошла"

#define  MCMD_GCMD_M	     0x50 //передать G-код в Marlin
#define  MCMD_GETPAR       0x51 //получить параметры
#define  MCMD_MSG           0x70 //сообщение


/* коды состояния */
#define  LPC_NOP            0x00  
#define  LPC_INIT		    0x01
#define  LPC_HAND           0x02  
//#define  LPC_MOVEKVANT_HAND 0x03  
#define  LPC_MOVEKVANT_XY   0x04  
#define  LPC_MOVEKVANT_XY2  0x05 //Движение по XY с произвольными регуляторами
#define  LPC_MOVEKVANT_XY2_2  0x06 //Движение по XY с произвольными регуляторами
//#define  LPC_HAND3          0x06  //Движение по XYZ  
#define  LPC_MOVEKVANT_HAND3 0x07  
#define  LPC_MOVEKVANT_3    0x08  
#define  LPC_MOVEKVANT_Y    0x09 //Движение по Y при поиске нуля
#define  LPC_TEST          0x10  
#define  LPC_MOVEKVANT_XYZ 0x11  
#define  LPC_MOVE_trapeze  0x1a /* Движение по трапеции от V=V0 X=x0 до V=0 X=x1 с ускорением a */ 


#define  LPC_FINDZERO_X	   0x20
#define  LPC_FINDZERO_XA   0x2a
#define  LPC_FINDZERO_Y	   0x21
#define  LPC_MOVE_X	       0x22 //Движение по X
#define  LPC_MOVE_Y	       0x23 //Движение по Y
#define  LPC_MOVE_Z	       0x24 //Движение по Z
#define  LPC_MOVE_XY       0x25 //Движение по XY
#define  LPC_MOVE_XY2      0x26 //Движение по XY с произвольными регуляторами
 

#define  LPC_WRITE_FLESH   0x100  


/************************************/

#define IN_ERR_0 0x00001
#define IN_ERR_1 0x00002
#define IN_ERR_2 0x00004
#define IN_PF__0 0x00008
#define IN_PF__1 0x00010
#define IN_PF__2 0x00020

#define IN_KDW_2 0x00040
#define IN_KUP_2 0x00080
#define IN_KUP_3 0x00100
#define IN_KDW_3 0x00200
#define IN_OP__0 0x00400
#define IN_OP__1 0x01000
#define IN_OP__2 0x02000
#define IN_KUP_0 0x04000
#define IN_KDW_0 0x08000
#define IN_KUP_1 0x10000
#define IN_KDW_1 0x20000

/************************************/
#define  IN_ZERO_X0       0x0001
#define  IN_ZERO_X1       0x0002
#define  IN_ZERO_Y        0x0004
#define  IN_ZEROZONE_X    0x0008
#define  IN_ZEROZONE_Y    0x0010
/*
#define  INX_XAVARIA       0x0010
#define  INY_YAVARIA       0x0010
#define  INZ_ZERO          0x0200
#define  INZ_MAX           0x0400
*/
#define  ERROR_AVARIA_X   0x01
#define  ERROR_AVARIA_Y   0x02
#define  ERROR_AVARIA_Z   0x04

#define LBUFF_SIZE 350 // размер буфера в миникадрах 5*2 = 10 байт 
#define MYLBUFF_SIZE 3500 // размер буфера в int

struct Master4_buf_item
{  short int dx[2];
//   unsigned short int nkvant;
   unsigned short int kadr;
   unsigned short int rep;
};

struct Master4_buf
{  
  int ibuf;
  int ifree;
  int Lbuff;
  struct Master4_buf_item b[LBUFF_SIZE];
};

struct ModelPar
{  float x;
   float v;
   float a;
   float inFUpr;
   float FUpr;
   float F;
   int kon;
   int ix;
   float dt;
   int xshift; //сдвиг координаты 
/***********/
   int type;         /* 0- Y, 1 - X0,X1  слейв*/
   float AvarKonXmin; // положение аварийного концевика - в мм слейв
   float AvarKonXmax; // положение аварийного концевика + в мм слейв
   float ZeroPosMin;     //положение нуля в мм	   (мастер)
   float ZeroPosMax;     //положение нуля в мм	   (мастер)
   float ZeroZonePosMin; //положение датчика "торможение" в мм (мастер)
   float ZeroZonePosMax; //положение датчика "торможение" в мм (мастер)
   float M;
   float Fadd;
   float Madd;
   float masht;
};

#endif //STOL4_CONFIG1

