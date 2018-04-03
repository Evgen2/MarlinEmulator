/* TCPconnection.hpp */
//����� TCPconnection
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
{   short int cmd0; //�������
    short int cmd; //�������
    short int ind; //��������
    unsigned char Buf[100];
};

struct Msg2
{   short int cmd0; //�������
    short int cmd; //�������
    short int ind; //��������
    unsigned char Buf[1500];
};
#endif

class TCPconnection
{
public:
   int sock0;
   int sock;
   struct sockaddr_in server;  //�����  �������
   struct sockaddr_in client;  //�����  �������
   int port;                   //����� ����� �� �������
   char IpTo[16];              //ip ����� ���������� ������� � char
   int timeout;                //������� �� ���������� � ��
   int timeoutAnswer;          //������� �� ���������� ������� � ��
   int timeLastRW;
   int nsend; // �������, ���
   int nresv; // ��������, pa�
   int nBsend; // ������ �������
   int nBresv; // ������ ��������
   int ierr;  // ��� ������, ���� ����
   int ncmd;  // ����� ��������� ������� 0-255
   short int indcmd;  //
   int nResend; // ������� ��� ������ ��������� �������
   int stat[16]; //����������
   TCPconnection(void) //����������� �� ���������
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
   ~TCPconnection(void) //����������
   { //if(sock > 0)
     //       soclose(sock);
   }
//������� ����������
   int createTCPconnection(char _IpTo[], int port, int timeout, int timeoutAnswer);
   int createTCPserverconnection(int port, int timeout, int timeoutAnswer);
//������� ����������
   int closeDPconnection(void)
   { //if(sock > 0)
     //       soclose(sock);
     sock = -1;
     return 0;
   }

   int AddRoute(void);

//������� ����� � ��������������
   int SendAndConfirm(char buf[], int len, char bufout[], int lenout);
//������� ����� � �������������� � ���������� ������ ������
   int SendAndConfirm2(char buf[], int len, char bufout[], int lenout, int shiftLansw, int size0, int FirstByte);
//������ ����������
   int GetInfo(void);
//������
   int Read(char bufin[], int len);
//������
   int Read(char bufin[], int len, int timeout);
//�������
   int Send(char buf[], int len);

};

class TCPclient
{
public:
	TCPconnection client;
	int id;
	int lifetime; // ����� �������� �������
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
