/* UDPconnection.hpp */
//класс UDPconnection
#ifndef UDPCONNECTION_DEF
#define UDPCONNECTION_DEF

#include <stdlib.h>
#include <string.h>
//#include <types.h>
//#include <sys\socket.h>
//#include <sys\ioctl.h>
//#include <nerrno.h>
#ifndef TCPV40HDRS
//#include <arpa\inet.h>
//#include <unistd.h> //soclose
#endif
//#include <netinet\in.h> //for sockaddr_in

#define CMD_MSG_DEFINED
struct Msg1
{   short int cmd0; //команда
    short int cmd; //команда
    short int ind; //параметр
    unsigned char Buf[100];
};

struct Msg2
{   short int cmd0; //команда
    short int cmd; //команда
    short int ind; //параметр
    unsigned char Buf[1500];
};

class UDPconnection
{
public:
   int sock;
   struct sockaddr_in server;  // сервер
   struct sockaddr_in client;  // клиент
   int port;                   //номер порта на сервере
   char IpTo[16];              //ip адрес удаленного сервера в char
   int timeout;                //таймаут на соединение в мс
   int timeoutAnswer;          //таймаут на повторение посылки в мс
   int nsend; // послано, раз
   int nresv; // получено, paз
   int nBsend; // байтов послано
   int nBresv; // байтов получено
   int ierr;  // код ошибки, если есть
   int ncmd;  // номер посланной команды 0-255
   short int indcmd;  //
   int nResend; // сколько раз делали повторную посылку
   int stat[16]; //статистика
   UDPconnection(void) //конструктор по умолчанию
   {  sock = 0;
//      memset(&server,0,sizeof(sockaddr_in));
//      memset(&client,0,sizeof(sockaddr_in));
      memset(&stat,0,sizeof(stat));
      port = 0;
      nsend = nresv = 0;
      nBsend = nBresv = 0;
      timeout = 1000;
      timeoutAnswer = 10;
      IpTo[0] = 0;
      ierr = 0;
      ncmd = 0;
      indcmd = 0x1a;
      nResend = 0;
   }
   ~UDPconnection(void) //деструктор
   { //if(sock > 0)
     //       soclose(sock);
   }
//создать соединение
   int createUDPconnection(char _IpTo[], int port, int timeout, int timeoutAnswer);
   int createUDPserverconnection(int port, int timeout, int timeoutAnswer);
//закрыть соединение
   int closeDPconnection(void)
   { //if(sock > 0)
     //       soclose(sock);
     sock = -1;
     return 0;
   }

   int AddRoute(void);

//послать буфер с подтверждением
   int SendAndConfirm(char buf[], int len, char bufout[], int lenout);
//послать буфер с подтверждением с переменной длиной ответа
   int SendAndConfirm2(char buf[], int len, char bufout[], int lenout, int shiftLansw, int size0, int FirstByte);
//запрос информации
   int GetInfo(void);
//читать
   int Read(char bufin[], int len);
//читать
   int Read(char bufin[], int len, int timeout);
//послать
   int Send(char buf[], int len);

};

#endif //UDPCONNECTION_DEF

