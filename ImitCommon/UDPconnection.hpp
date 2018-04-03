/* UDPconnection.hpp */
//����� UDPconnection
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

class UDPconnection
{
public:
   int sock;
   struct sockaddr_in server;  // ������
   struct sockaddr_in client;  // ������
   int port;                   //����� ����� �� �������
   char IpTo[16];              //ip ����� ���������� ������� � char
   int timeout;                //������� �� ���������� � ��
   int timeoutAnswer;          //������� �� ���������� ������� � ��
   int nsend; // �������, ���
   int nresv; // ��������, pa�
   int nBsend; // ������ �������
   int nBresv; // ������ ��������
   int ierr;  // ��� ������, ���� ����
   int ncmd;  // ����� ��������� ������� 0-255
   short int indcmd;  //
   int nResend; // ������� ��� ������ ��������� �������
   int stat[16]; //����������
   UDPconnection(void) //����������� �� ���������
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
   ~UDPconnection(void) //����������
   { //if(sock > 0)
     //       soclose(sock);
   }
//������� ����������
   int createUDPconnection(char _IpTo[], int port, int timeout, int timeoutAnswer);
   int createUDPserverconnection(int port, int timeout, int timeoutAnswer);
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

#endif //UDPCONNECTION_DEF

