/* TCPconnection.hpp */
//класс TCPconnection
#ifndef TCPCONNECTION_DEF
  #define TCPCONNECTION_DEF

#ifdef _WIN32
#define  _CRT_SECURE_NO_WARNINGS 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define _AFXDLL
 #include <afxsock.h>		// MFC socket extensions
 #include <WinSock.h>
 #include <process.h>    /* _beginthread, _endthread */
 #define socklen_t int
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define SOCKET_ERROR (-1)
#define closesocket close

inline int Sleep(int ms)
{   usleep(1000*ms);
    return 0;
}
#endif


#define HAND_SHAKE_INP           "TCPiptEsT"
#define HAND_SHAKE_OUT           "ipTCPTeSt"
#define CMD_HAND_SHAKE  0
#define CMD_ECHO        1

#ifndef CMD_MSG_DEFINED
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
#endif

class TCPconnection
{
public:
   int sock0;
   int sock;
   struct sockaddr_in server;  //адрес  сервера
   struct sockaddr_in client;  //адрес  клиента
   int port;                   //номер порта на сервере
   char IpTo[16];              //ip адрес удаленного сервера в char
   int timeout;                //таймаут на соединение в мс
   int timeoutAnswer;          //таймаут на повторение посылки в мс
   int timeLastRW;
   int nsend; // послано, раз
   int nresv; // получено, paз
   int nBsend; // байтов послано
   int nBresv; // байтов получено
   int ierr;  // код ошибки, если есть
   int ncmd;  // номер посланной команды 0-255
   short int indcmd;  //
   int nResend; // сколько раз делали повторную посылку
   int stat[16]; //статистика
   TCPconnection(void) //конструктор по умолчанию
   {  sock = sock0 = 0;
//      memset(&server,0,sizeof(sockaddr_in));
//      memset(&client,0,sizeof(sockaddr_in));
      memset(&stat,0,sizeof(stat));
      port = 0;
      nsend = nresv = 0;
      nBsend = nBresv = 0;
      timeout = 200000;
      timeoutAnswer = 2000;
      IpTo[0] = 0;
      ierr = 0;
      ncmd = 0;
      indcmd = 0x1a;
      nResend = 0;
	  timeLastRW = 0;
   }
   ~TCPconnection(void) //деструктор
   { //if(sock > 0)
     //       soclose(sock);
   }
//создать соединение
   int createTCPconnection(char _IpTo[], int port, int timeout, int timeoutAnswer);
   int createTCPserverconnection(int port, int timeout, int timeoutAnswer);
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

class TCPclient
{
public:
	TCPconnection client;
	int id;
	int lifetime; // время ожидания клиента
	char buffer[1524];

	TCPclient(void)
	{  id = 0;
	   lifetime = 3600000;
	}
	int CommandHandle(int buflen);
};

int GetClock(void);
int GetClockInit(void);
int ShowMyIp(void);
int InitTCPIP(void);

#endif
//TCPCONNECTION_DEF
