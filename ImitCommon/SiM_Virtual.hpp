/* SiM_Virtual.hpp */
#ifndef SIM_VIRTUAL_H
#define SIM_VIRTUAL_H

#include "SiM.h"
#include "UDPconnection.hpp"
#include "TCPconnection.hpp"

class SiM_VirtualSetup
{
  public:
   int config; // ������������: 0 - ��������, 1 - LS250, 2 - LS100
   int subconfig; //���������������
   int type;      // 0  - UDP 1 - TCP 2 - COM
    int base_port;
	int tcp_port;
    char ipaddr[32];
    char ReadBuff[1500];
    char WriteBuff[1500];
    int Nwrite; 
	int Nread;
    UDPconnection lpcKl; //��������� lpc-0
    UDPconnection lpc;   //��������� lpc-1
    TCPconnection tcp;   //��������� 
	unsigned char socKl;
	unsigned char soc;
    char TcpReadBuff[1500];
    char TcpWriteBuff[1500];

    HANDLE Hcom;  // ����� ��������
	int Ncom;     // ����� ��������

   SiM_VirtualSetup(void)  //��������� �����������
   {  config = 0;
      subconfig = 0;
      base_port = 0x1a70;
	  tcp_port = 8876;
      strcpy(ipaddr,"192.168.1.122");
	  Nwrite = 0;
	  Nread = 0;
	  soc = 0;
	  socKl = 1;
	  type = -1;
	  Hcom = NULL;
	  Ncom = 0;
   }
   int Init(int debug_mode);
   int InitUDP(int debug_mode);
   int InitTCP(int debug_mode);
   int SendTCP(unsigned char *buf, int l);
   int InitCOM(int NComPort, int Bitrate, int debug_mode);
   int COMirqImit(void);
   int InitConnection(void);
   int Read(void);
   int CallBack(void);
   int Work(void);
   int Send(void);
};


#endif //SIM_VIRTUAL_H
