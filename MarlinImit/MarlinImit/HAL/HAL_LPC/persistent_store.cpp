/* persistent_store.cpp */

#include <string.h>

#include "../../inc/MarlinConfig.h"

#include "../persistent_store_api.h"

//??
//#include "types.h"
//#include "language.h"
//#include "utility.h"

#ifndef WIN32
#include <Net_Config.h>
#endif
#include "LPC17xx.h"		// Keil: Register definition file for LPC17xx
//#include <RTL.h>

//#include "chanfs/diskio.h"
//#include "chanfs/ff.h"

#ifdef TARGET_LPC1768
#if ENABLED(EEPROM_SETTINGS)

extern "C" void * GetCfgPtrFromFlash(void);
extern "C" int WriteCfg_To_Flash(void *pconfig, int len);

#define MAX_FLASH_SIZE 1024
//debug
char flash_buf[MAX_FLASH_SIZE];
//uint16_t flash_crs[MAX_FLASH_SIZE];

namespace HAL {
namespace PersistentStore {

static char * _fbuf=NULL;
static char * p_fbuf=NULL;
static int flash_err = 0;	
static int flash_write = 0;	
	
bool access_start() {
//todo
  bool res = 0;
	flash_err = 0;
	flash_write = 0;
#if 1	
#if defined(WIN32)
	if( _fbuf == NULL)
			_fbuf = (char *)calloc(1,MAX_FLASH_SIZE);
#else
	if( _fbuf == NULL)
	   _fbuf = (char *)alloc_mem (MAX_FLASH_SIZE + 16);
#endif
   if(_fbuf == NULL)
   {   flash_err = 1;
		 return false;
	 }
   p_fbuf = (char *) GetCfgPtrFromFlash();
#else
//debug
	 _fbuf = flash_buf;
	  p_fbuf =  flash_buf;
#endif
  return (res == 0);
}

bool access_finish()
{   int rc; 
//  f_close(&eeprom_file);
//  f_unmount("");
#if 1
	  if(flash_write > 0)
		{
/* проверяем отличие чтоб лишний раз не писать */
      rc = memcmp(_fbuf, p_fbuf, flash_write);
			if(rc) 
			{  rc = WriteCfg_To_Flash((void *)_fbuf, flash_write);
//					rc0 = rc;
			}
		}
#if defined(WIN32)
   free(_fbuf);
#else
   free_mem ((OS_FRAME *)_fbuf);
#endif
   _fbuf = NULL;
#else
//debug		
#endif	
	if(flash_err == 0)
			return true;
	else
			return false;
}

bool write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc) {
  int bytes_written = 0;
	
	if(pos+size >= MAX_FLASH_SIZE)
	{	  flash_err = 2;
		  return false; 
	}
//  f_lseek(&eeprom_file, pos);
//  f_write(&eeprom_file, (void *)value, size, &bytes_written);
   memcpy(_fbuf+pos,(void *)value,size); 
	bytes_written = size;
  crc16(crc, value, size);
	
  pos = pos + size;
	if(flash_write < pos)
			flash_write = pos;
  return (bytes_written == size);
}

bool read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc, const bool writing/*=true*/) {
  int bytes_read = 0;
//  f_lseek(&eeprom_file, pos);
//  f_read(&eeprom_file, (void *)value, size, &bytes_read);
	if(pos+size >= MAX_FLASH_SIZE)
	{	  flash_err = 3;
		  return false; 
	}
	
  memcpy((void *)value, (void *)(p_fbuf+pos),size); 
  crc16(crc, value, size);

  pos = pos + size;
  return bytes_read != size;  // return true for any error
}

}
}

#endif // EEPROM_SETTINGS
#endif // ARDUINO_ARCH_AVR
