/* HAL_LPC_Serial.cpp */
//  class LPC1768Serial 

#include "../../Marlin.h"

#include "LPC1768Serial.h"
#include "uart.h"

extern "C" void setupUart(void);
extern void send_tcp(char *buf, int len);
extern void send_udp(char *buf, int len);

LPC1768Serial customizedSerial;
extern volatile int tcp_connect_on;
extern int UdpOn;

void setupUart(void)
{
	  customizedSerial = LPC1768Serial(USBTX, USBRX); 

}

void on_get_tcp_init(void)
{
				 customizedSerial.bTx.ibuf = customizedSerial.bTx.ifree = 0;	 
}
void on_get_tcp(char *buf, int len)
{   customizedSerial.bRx.Add(buf,len);
}

void LPC1768Serial::begin(int baudrate)
{
	  baud(baudrate);
	  bRx.Init(bufin, RX_BUFFER_SIZE);
	  bTx.Init(bufout, TX_BUFFER_SIZE);
			serial_irq_handler(&_serial, SerialA::_irq_handler, (uint32_t)this);
	
    attach(this, &LPC1768Serial::on_get, RxIrq);
    attach(this, &LPC1768Serial::on_put, TxIrq);
}

int LPC1768Serial::read_0(void) {
    int v = -1;
    int l;	
	  l = bRx.GetLbuf();
	  if(l > 0)
		{  v = bRx.Get();
		}
	
   return v;
}

void LPC1768Serial::on_get(void)
{
	char byte;
  byte = SerialA::_getc();		
	bRx.Add(byte);
}

void LPC1768Serial::on_put(void)
{   int l;
	  int c;
	if(!tcp_connect_on && !UdpOn)
	{
	  l = bTx.GetLbuf();
	  if(l > 0)
		{	 c = bTx.Get();
			_putc(c);
		}
	}
}

void LPC1768Serial::flush(void) 
{
	;
//todo
}

uint8_t LPC1768Serial::available(void) {

	return bRx.GetLbuf();
}

void LPC1768Serial::write_0(const uint8_t c) {
//	 Serial::_putc(c);
      write_0(&c, 1);	
}

void LPC1768Serial::write_0(const int c)
{
      write_0((uint8_t *)&c, 1);	
//	Serial::_putc(((uint8_t)c));
}

void LPC1768Serial::write_0(const char* str) 
{ // while (*str) write_0(*str++);
   size_t l;
   l = strlen(str);	
	 write_0((const uint8_t*)str, l);
}
void LPC1768Serial::write_0(const uint8_t * buffer, size_t size)
 { uint8_t ch;
	 int i,l, needsend;
	 if(size <= 0)
	  return;
	 if(tcp_connect_on || UdpOn)
	 { 
		 bTx.Add((char *)buffer, size);
		 needsend = 0;
//дебуг
	 for(i=0; i<size; i++)
	 {  
	    ch = buffer[i];
	    SerialA::_putc(ch);
	 }

		 for(i=0; i<size; i++)
		   {  if(buffer[i] == 0x0a)
					needsend = 1;
			 } 
			 if(needsend)
			 { 
				 if(UdpOn)
					 send_udp(bTx.pbuf, bTx.ifree);

				 else
						send_tcp(bTx.pbuf, bTx.ifree); 

				 bTx.ibuf = bTx.ifree = 0;	 
				 
			 }
			 return;
	 }
/* debug todo todo todo todo todo !!!!!!!!!!!!!!!!!!!!!!   */
#if 1
	 for(i=0; i<size; i++)
	 {  l=i;
		 if(writeable())
		 {
	    ch = *buffer++;
		  SerialA::_putc(ch);
		 } else i--;
	 }
#else	 
	 __disable_irq(); 
	l = bTx.GetLbuf();
	if(writeable() && l == 0)
	{   ch = *buffer++;
		  size--;
		  if(size > 0) 
		     bTx.Add((char *)buffer, size);
      __enable_irq();			
		  Serial::_putc(ch);
	} else {
		 if(l >= TX_BUFFER_SIZE-1)
		 { 
				__enable_irq();			
			 do
			 {
				 l = bTx.GetLbuf();
			 } while(l >= TX_BUFFER_SIZE-1);
				__disable_irq(); 
		}
		    bTx.Add((char *)buffer, size);
			__enable_irq();
	}
#endif //0	
}

/// imports from print.h


void LPC1768Serial::print(char c, int base) {
    print((long)c, base);
}

void LPC1768Serial::print(unsigned char b, int base) {
    print((unsigned long)b, base);
}

void LPC1768Serial::print(int n, int base) {
    print((long)n, base);
}

  void LPC1768Serial::print(unsigned int n, int base) {
    print((unsigned long)n, base);
  }

  void LPC1768Serial::print(long n, int base) {
    if (base == 0)
      write_0((int)n);
    else if (base == 10) {
      if (n < 0) {
        print('-');
        n = -n;
      }
      printNumber(n, 10);
    }
    else
      printNumber(n, base);
  }

  void LPC1768Serial::print(unsigned long n, int base) {
    if (base == 0) write_0((int)n);
    else printNumber(n, base);
  }

  void LPC1768Serial::print(double n, int digits) {
    printFloat(n, digits);
  }

// Private Methods

  void LPC1768Serial::printNumber(unsigned long n, uint8_t base) {
    if (n) {
      unsigned char buf[8 * sizeof(long)]; // Enough space for base 2
      int8_t i = 0;
      while (n) {
        buf[i++] = n % base;
        n /= base;
      }
      while (i--)
        print((char)(buf[i] + (buf[i] < 10 ? '0' : 'A' - 10)));
    }
    else
      print('0');
  }

  void LPC1768Serial::printFloat(double number, uint8_t digits) {
		int toPrint;
    // Handle negative numbers
    if (number < 0.0) {
      print('-');
      number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
      rounding *= 0.1;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits) {
      print('.');
      // Extract digits from the remainder one at a time
      while (digits--) {
        remainder *= 10.0;
        toPrint = int(remainder);
        print(toPrint);
        remainder -= toPrint;
      }
    }
  }

void LPC1768Serial::printf(const char *format, ...)
{ char RxBuffer[256];
  va_list vArgs;
  va_start(vArgs, format);
  int length = vsnprintf(RxBuffer, 256, format, vArgs);
  va_end(vArgs);
  if (length > 0 && length < 256) {
    for (int i = 0; i < length; ++i)
      write_0(RxBuffer[i]);
  }
}

void LPC1768Serial::print(const char* str) 
{ write_0(str); 
}
	

// добавим из прерывания void UART_IRQHandler (int nport) байт в буфер
int AddUART_RX_to_buffer(int nport, char byte)
{
//todo
//  return roz.bIn[nport].Add(byte);	
return 0;
	
}

int AddUART_TX_to_buffer(int nport, char byte)
{
//todo
//return roz.bOut[nport].Add(byte);
//todo
return 0;
}

int UART_RX_bufferLength(int nport)
{ //  return roz.bIn[nport].GetLbuf();
//todo
return 0;
}

int UART_Get_buffer(int nport)
{
//   return roz.bIn[nport].Get();
//todo
return 0;

}

void UART_Init_buffer(int nport)
{
//todo
//  roz.bIn[nport].ibuf = roz.bIn[nport].ifree = 0;	 
	  return; 
}

