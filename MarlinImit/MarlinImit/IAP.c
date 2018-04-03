/*****************************************************************************
 *   IAP.c:  IAP Flesh API for NXP LPC23xx/24xx Family Microprocessors
 *
******************************************************************************/
//#include "LPC23xx.h"                        /* LPC23xx/24xx definitions */
#include "LPC17xx.h"      // Keil: Register definition file for LPC17xx
#ifndef WIN32

#include "iap.h"
#define CRYSTAL_FREQUENCY SystemCoreClock 
//#define CRYSTAL_FREQUENCY Fosc * 6
   
//23xx #define m_IAP_ENTRY(a, b) ((void (*)(tU32[], tU32[]))(0x7ffffff1))(a, b)   
#define m_IAP_ENTRY(a, b) ((void (*)(tU32[], tU32[]))(0x1fff1ff1))(a, b)   

int WriteCfg_To_Flash(void *pconfig, int len);

//--------------------------------------------------------------------------------------------------
// Точка входа в IAP
//23xx #define IAP_LOCATION 0x7FFFFFF1
#define IAP_LOCATION 0x1FFF1FF1
typedef void (*IAP)(unsigned long [],unsigned long []);
static IAP IapEntry = (IAP)IAP_LOCATION;


// Коды команд IAP
#define IAP_CMD_PREPARE 50
#define IAP_CMD_WRITE 51
#define IAP_CMD_ERASE 52
//--------------------------------------------------------------------------------------------------
// Коды статуса IAP
#define CMD_SUCCESS 0
#define BUSY 11

//--------------------------------------------------------------------------------------------------
// Вызов команды IAP
/* __inline ???? */
unsigned long IapCommand (
        unsigned long cmd,
        unsigned long p0,
        unsigned long p1,
        unsigned long p2,
        unsigned long p3,
        unsigned long *r0)
{
        unsigned long command[5]; /* = {cmd, p0, p1, p2, p3}; */
        unsigned long result[2]={1,1};
     	unsigned long int enabled_interrupts;

		command[0] = cmd;
		command[1] = p0;
		command[2] = p1;
		command[3] = p2;
		command[4] = p3;
        // На время выполнения команды IAP запрещаю прерывания
 __disable_irq();      
//	enabled_interrupts = VICIntEnable;  //disable all interrupts
//	VICIntEnClr        = enabled_interrupts;

        // Запускаю команду IAP
        IapEntry (command, result);

        // Восстанавливаю прерывания
  __enable_irq();      
//	VICIntEnable = enabled_interrupts;  //restore interrupt enable register
        // Возвращаю результат
        if (r0)
        {
                *r0 = result[1];
        }
        return result[0];
}

/***************************************************************************** 
 * 
 * Description: 
 *    Performs the actual IAP command, based on input command stored in 
 *    'pCommand'. The PLL, MAM, and VIC are disabled during the command 
 *    and restored afterwards. 
 *    This code must run in THUMB mode, since the IAP interface assumes this! 
 *     
 * Params: 
 *    [in]    pCommand - pointer to command vector (up to five tU32 words) 
 *    [inout] pResult  - pointer to where the result can be stored 
 * 
 ****************************************************************************/  
static void  
iapCommand(tU32 *pCommand, tU32 *pResult)  
{  
//    int OldIntEnable, t0;
//	OldIntEnable = VICIntEnable;
//__disable_irq();      

//	VICIntEnable = 0;
//	t0 = T1TC;
//	while(T1TC-t0<7200000)
//	{ if(VICIRQStatus == 0) break;
//	} 

  //perform IAP command  
  m_IAP_ENTRY(pCommand, pResult);  

//__enable_irq();      
//	VICIntEnable = OldIntEnable;
}

/*
 ***************************************************************************** 
 * Implementation of public functions 
 ****************************************************************************/  
  
/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Select Sector, which prepares the sectors for 
 *    write operations. 
 *     
 * Params: 
 *    [in]    sector1 - start sector 
 *    [in]    sector2 - end sector (must be equal or larger to sector1) 
 * 
 * Returns:  
 *    IAP status code (see list in iap.h) 
 * 
 ****************************************************************************/  
tU32  
iapSelectSector(tU8 sector1, tU8 sector2)  
{  
    tU32 parameter[3];  
    tU32 result[1];  
  
    parameter[0] = IAP_SELSECTOR; /* 50 */ 
    parameter[1] = sector1;  
    parameter[2] = sector2;  
    iapCommand(parameter, result);  

      
    return result[0];  
}  

# /***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Copy RAM to FLASH 
 *     
 * Params: 
 *    [in] destAddress - desitnation address within the FLASH memory 
 *                       Must be on 256 byte boundary. 
 *    [in] srcAddress  - address to RAM data block (word boundary needed). 
 *    [in] length      - length of data block (256, 512, 1024 or 4096) 
 * 
 * Returns:  
 *    IAP status code (see list in iap.h) 
 * 
 ****************************************************************************/  
tU32  
iapRamToFlash(tU32 destAddress, tU32 srcAddress, tU32 length)  
{  
    tU32 parameter[5];  
    tU32 result[1];  
    parameter[0] = IAP_RAMTOFLASH;  /* 51 */
    parameter[1] = destAddress;  
    parameter[2] = srcAddress;  
    parameter[3] = length;  
    parameter[4] = CRYSTAL_FREQUENCY / 1000;  
    iapCommand(parameter, result);  
    return result[0];  
}  
  
  
/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Erase Sector 
 *     
 * Params: 
 *    [in] sector1 - start sector 
 *    [in] sector2 - end sector (must be equal or larger to sector1) 
 * 
 * Returns:  
 *    IAP status code (see list in iap.h) 
 * 
 ****************************************************************************/  
tU32  
iapEraseSector(tU8 sector1, tU8 sector2)  
{  
    tU32 parameter[4];  
    tU32 result[1];  
      
    parameter[0] = IAP_ERASESECTOR;  
    parameter[1] = sector1;  
    parameter[2] = sector2;  
    parameter[3] = CRYSTAL_FREQUENCY / 1000;  
__disable_irq();      

    iapCommand(parameter, result);  

__enable_irq();      
      
    return result[0];  
}  
  
  
/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Blank Check Sectors 
 *     
 * Params: 
 *    [in] sector1 - start sector 
 *    [in] sector2 - end sector (must be equal or larger to sector1) 
 * 
 * Returns:  
 *    IAP status code (see list in iap.h) 
 * 
 ****************************************************************************/  
tU32  
iapBlankCheck(tU8 sector1, tU8 sector2)  
{  
    tU32 parameter[3];  
    tU32 result[1];  
      
    parameter[0] = IAP_BLANKCHK;  
    parameter[1] = sector1;  
    parameter[2] = sector2;  
    iapCommand(parameter, result);  
      
    return result[0];  
}  
  
/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Read Part ID 
 *     
 * Returns:  
 *    The Part ID, or 0 (if command failed) 
 * 
 ****************************************************************************/  
tU32  
iapReadPartID(void)  
{  
    tU32 parameter[1];  
    tU32 result[2];  
      
    parameter[0] = IAP_READPARTID;  
    iapCommand(parameter, result);  
      
    if (result[0] == IAP_CMD_SUCCESS)  
      return result[1];  
    else  
      return 0;  
}  


/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Read Boot Version 
 *     
 * Returns:  
 *    The boot code version, or 0 (if command failed) 
 * 
  ****************************************************************************/  
tU32  
iapReadBootVer(void)  
{  
    tU32 parameter[1];  
    tU32 result[2];  
      
   parameter[0] = IAP_BOOTCODEID;  
    iapCommand(parameter, result);  
      
    if (result[0] == IAP_CMD_SUCCESS)  
      return result[1];  
    else  
      return 0;  
}  
  
  
/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Compare 
 *     
 * Params: 
 *    [in] destAddress - desitnation address within the FLASH memory 
 *                       Must be on word boundary. 
 *    [in] srcAddress  - address to RAM data block (word boundary needed). 
 *    [in] length      - length of data block (must be a multiple of 4) 
 * 
 * Returns:  
 *    IAP status code (see list in iap.h) 
 * 
 ****************************************************************************/  
tU32  
iapCompare(tU32 destAddress, tU32 srcAddress, tU32 length)  
{  
    tU32 parameter[4];  
    tU32 result[1];  
     
    parameter[0] = IAP_COMPARE;  
    parameter[1] = destAddress;  
    parameter[2] = srcAddress;  
    parameter[3] = length;  
    iapCommand(parameter, result);  
      
    return result[0];  
}  
  
  
/***************************************************************************** 
 * 
 * Description: 
 *    Performs the IAP command: Reinvoke ISP 
 *     
 ****************************************************************************/  
void  
iapReinvokeIsp(void)  
{  
    tU32 parameter[1];  
    tU32 result[1];  
      
    parameter[0] = IAP_REINVOKE_ISP;  
    iapCommand(parameter, result);  
}  

// LPC2378 static void * destAddress = (int *)0x7d000; //0x7d000;
// LPC1768          Sector29:   0x00078000 - 0x0007FFFF        32K
static void * destAddress = (int *)0x78000; //0x7d000;

#if 0
int IAP_test(void)
{   int rc;
	int curr_sec = 27; //последний в 2378
	int length;
	int i;
static int mas[256];
    rc  = iapReadPartID();

//готовлю сектор к стиранию
    rc = iapSelectSector(curr_sec,curr_sec);  

// стираю сектор
iap_erase:
    rc =  iapEraseSector(curr_sec, curr_sec);  
    if(rc == IAP_BUSY) 
           goto iap_erase;
//готовлю сектор к записи
    rc = iapSelectSector(curr_sec,curr_sec);  
	length = 256;
	for(i=0; i< 256/4; i++)
	                   mas[i] = i;
// пишу сектор
iap_RamToFlash:
   rc = iapRamToFlash((tU32)destAddress, (tU32)mas, length);  
    if(rc == IAP_BUSY) 
           goto iap_RamToFlash;

	return rc;
}
#endif //0


/***********************************/
/*    Запись конфига во флеш       */
/***********************************/
int iap_rc[4];

int WriteCfg_To_Flash(void *pconfig, int len)
{  int rc=0;
//   int curr_sec = 27; //27; //последний сектор в 2378
   int curr_sec = 29; //27; //последний сектор в 1768

// подгоняю длину данных к допустимой
   if (len < 256)
                        len = 256;
   else if(len < 512)
                        len = 512;
   else if(len < 1024)
                        len = 1024;
   else
                        len = 4096;


 // Готовлю сектор к стиранию
                iap_prepare1:
                switch(IapCommand (
                        IAP_CMD_PREPARE,
                        curr_sec,
                        curr_sec,
                        0,
                        0,
                        0))
                {
                case CMD_SUCCESS:                       // Порядок - иду дальше
                        break;
                case BUSY:                              // Занято - пробую еще раз
                        goto iap_prepare1;
                default:                                // Ошибка - вылетаю
                        return 0;
                }

                // Стираю сектор
                iap_erase:
                switch(IapCommand (
                        IAP_CMD_ERASE,
                        curr_sec,
                        curr_sec,
                        CRYSTAL_FREQUENCY  / 1000,                   // Тактовая частота процессора в килогерцах
                        0,
                        0))
                {
                case CMD_SUCCESS:                       // Порядок - иду дальше
                        break;
                case BUSY:                              // Занято - пробую еще раз
                        goto iap_erase;
                default:                                // Ошибка - вылетаю
                        return 0;
                }

                // Готовлю сектор к записи
                iap_prepare2:
                switch(IapCommand (
                        IAP_CMD_PREPARE,
                        curr_sec,
                        curr_sec,
                        0,
                        0,
                        0))
                {
                case CMD_SUCCESS:                       // Порядок - иду дальше
                        break;
                case BUSY:                              // Занято - пробую еще раз
                        goto iap_prepare2;
                default:                                // Ошибка - вылетаю
                        return 0;
                }

                // Записываю сектор
                iap_write:
                switch(IapCommand (
                        IAP_CMD_WRITE,
                        (unsigned long)destAddress,
                        (unsigned long)pconfig,
                        len,
                        CRYSTAL_FREQUENCY  / 1000,                   // Тактовая частота процессора в килогерцах
                        0))
                {
                case CMD_SUCCESS:                       // Порядок - иду дальше
                        break;
                case BUSY:                              // Занято - пробую еще раз
                        goto iap_write;
                default:                                // Ошибка - вылетаю
                        return 0;
                }

	iap_rc[3] = rc;

    return rc;
}

int WriteCfg_To_Flash_old(void *pconfig, int len)
{  int rc;
   int curr_sec = 27; //последний сектор в 2378

// подгоняю длину данных к допустимой
   if (len < 256)
                        len = 256;
   else if(len < 512)
                        len = 512;
   else if(len < 1024)
                        len = 1024;
   else
                        len = 4096;

//готовлю сектор к стиранию
    rc = iapSelectSector(curr_sec,curr_sec);  

	iap_rc[0] = rc;

// стираю сектор
iap_erase:
    rc =  iapEraseSector(curr_sec, curr_sec);  
    if(rc == IAP_BUSY) 
           goto iap_erase;

	iap_rc[1] = rc;

//готовлю сектор к записи
    rc = iapSelectSector(curr_sec,curr_sec);  
	iap_rc[2] = rc;

// пишу сектор
iap_RamToFlash:
   rc = iapRamToFlash((tU32)destAddress, (tU32)pconfig, len);  
    if(rc == IAP_BUSY) 
           goto iap_RamToFlash;
	iap_rc[3] = rc;

    return rc;
}
/***********************************/
/* получить адрес конфига из флеша */
/***********************************/
void * GetCfgPtrFromFlash(void)
{  return destAddress;
}


#else 
// WIN32
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

static void * destAddress = NULL; //0x7d000;
static char EEPROMfname[]="eeprom.dat";
void IARemul_cleanup(void);

/***********************************/
/* получить адрес конфига из флеша */
/***********************************/
void * GetCfgPtrFromFlash(void)
{  
	if(destAddress == NULL)
	{   FILE *fp;
	    int rc;
		destAddress = calloc(1024,4);
	    fp = fopen(EEPROMfname,"rb");
		if(fp)
		{   rc = fread(destAddress,4,1024,fp);
			fclose(fp);
		}
		atexit(IARemul_cleanup);
	}
	return destAddress;
}

int WriteCfg_To_Flash(void *pconfig, int len)
{   FILE *fp;
    int rc;
   fp = fopen(EEPROMfname,"wb");
	if(fp)
	{   rc = fwrite(pconfig,1,len,fp);
		fclose(fp);
		memcpy(destAddress,pconfig, len);
	}
	return 0;
}

void IARemul_cleanup(void)
{   FILE *fp;
    int rc;
	if(destAddress)
	{
	    fp = fopen(EEPROMfname,"wb");
		if(fp)
		{   rc = fwrite(destAddress,4,1024,fp);
		}
		fclose(fp);
	}
}

#endif

