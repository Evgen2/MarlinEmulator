/* LPC1768Serial.h */

#ifndef LPC1768SERIAL_H
#define LPC1768SERIAL_H

#include "mbed.h"
#include "Serial.h"
#include "..\..\mybuffer.hpp"

using namespace mbed;

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

  // Define constants and variables for buffering incoming serial data.  We're
  // using a ring buffer (I think), in which rx_buffer_head is the index of the
  // location to which to write the next incoming character and rx_buffer_tail
  // is the index of the location from which to read.
  // 256 is the max limit due to uint8_t head and tail. Use only powers of 2. (...,16,32,64,128,256)
  #ifndef RX_BUFFER_SIZE
    #define RX_BUFFER_SIZE 128
  #endif
  #ifndef TX_BUFFER_SIZE
    #define TX_BUFFER_SIZE 32
  #endif
  
  
#if 0
  struct ring_buffer_r {
    unsigned char buffer[RX_BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
  };

  #if TX_BUFFER_SIZE > 0
    struct ring_buffer_t {
      unsigned char buffer[TX_BUFFER_SIZE];
      volatile uint8_t head;
      volatile uint8_t tail;
    };
  #endif  
		extern ring_buffer_r rx_buffer;
    #if TX_BUFFER_SIZE > 0
      extern ring_buffer_t tx_buffer;
    #endif
#endif //0

class LPC1768Serial : public SerialA 
{ //: public Stream

    public:
  char bufin[RX_BUFFER_SIZE];
  char bufout[TX_BUFFER_SIZE];
		
  myBuffer bRx;   /* buffer UART RX */ 
  myBuffer bTx;  /* buffer UART TX */ 
			
      LPC1768Serial(void) {};
			LPC1768Serial (PinName tx, PinName rx, const char *name=NULL) : SerialA(tx, rx)
			{ ;
				
			}
      void begin(int baudrate);
      static void end();
      static int peek(void);
      int read_0(void);
//      int read(void); // warning:  #997-D: function "mbed::FileHandle::read(void *, std::size_t)"  is hidden by "LPC1768Serial::read"  -- virtual function override intended?
      static void flush(void);
      uint8_t available(void);
      static void checkRx(void);
      void write_0(const uint8_t c);
      void write_0(const int c);
      #if TX_BUFFER_SIZE > 0
        static uint8_t availableForWrite(void);
        static void flushTX(void);
      #endif

    private:
      void printNumber(unsigned long, const uint8_t);
      void printFloat(double, uint8_t);

    public:
//      FORCE_INLINE 
         void write_0(const char* str);
//      FORCE_INLINE 
        void write_0(const uint8_t* buffer, size_t size);
//    static FORCE_INLINE void print(const String& s) { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
//      FORCE_INLINE void print(const char* str) { write_0(str); }
      void print(const char* str);

      void print(char, int = BYTE);
      void print(unsigned char, int = BYTE);
      void print(int, int = DEC);
      void print(unsigned int, int = DEC);
      void print(long, int = DEC);
      void print(unsigned long, int = DEC);
      void print(double, int = 2);

  void printf(const char *format, ...);

  void println(const char value[]) {
    printf("%s\n" , value);
  }
  void println(char value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(unsigned char value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(int value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(unsigned int value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(long value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(unsigned long value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(float value, int round = 6) {
    printf("%f\n" , value );
  }
  void println(double value, int round = 6) {
    printf("%f\n" , value );
  }
  void println(void) {
    print('\n');
  }


      operator bool() { return true; };
			void on_get(void);
			void on_put(void);
			
  };

  extern  LPC1768Serial customizedSerial;

  #define MYSERIAL0 customizedSerial
  #define MYSERIAL  customizedSerial


#endif //LPC1768SERIAL_H
