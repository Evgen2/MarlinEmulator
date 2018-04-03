/* SiM_VirtHandle.cpp */
#include "stdafx.h"
#include "SiM.h"
#include "SiM_Virtual.hpp"


/*+---------------------------------+*/
/*| External function prototypes.   |*/
/*+---------------------------------+*/
int ShowMyIp(void);

extern "C" int mystuff(int par);      //  main cicle
extern "C" void myInit(int state);    //  init
extern "C" int SiM(void);
extern int SetTimerIncr(int raz, int mc);

/*+---------------------------------+*/
/*| Internal function prototypes.   |*/
/*+---------------------------------+*/

int TimerCOn(void);
void MainLoopWork(void * param);
void TimerWork(void * param);
void ComWork(void * param);
int SimStart(void);

//FILE *fp;


int SiM_VirtualSetup::Init(int debug_mode)
{	int rc,i,t0, dt,idd;
    uintptr_t hid;
#if USE_COMPORT
extern int use_wifi;
#endif
	if(type == -1)
	{	printf("Call first InitUDP/TCP/COM\n");
	    return(-1);
	}

	printf("Timer calibration\n");
#if USE_COMPORT
	use_wifi = 0;
#endif
	myInit(0);
	SimStart();
	t0 = clock();
    for(i=0; ; i++)
	{   dt =clock()-t0;
	    if(dt > 1000)
			     break;
		 SiM();
		 mystuff(1);
	}
	printf("%i steps\n", i);
        SetTimerIncr(i, dt);
//	fp = fopen("s.log", "w");
#if USE_COMPORT
	use_wifi = 1;
#endif
	SimStart();

    hid = _beginthread(TimerWork,0,(void *) &idd);
	Sleep(10);
    hid = _beginthread(MainLoopWork,0,(void *) &idd);

	myInit(1);

   return 0;
}

int SiM_VirtualSetup::InitUDP(int debug_mode)
{  int rc;
    WSADATA wsaData = {0};

/* for(i=0; i< 1024; i++)
{ double U, U1, Tz;
  U = i;
  Tz = LinInterpolT(U);
  U1 = LinInterpolU(Tz);
  printf("U=%f T=%f U1 = %f\n", U, Tz, U1);
}
exit(1);
*/
  // Initialize Winsock
    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc != 0) 
	{   printf("WSAStartup failed: %d\n", rc);
        return 1;
    }

	ShowMyIp();

    rc = lpcKl.createUDPserverconnection(base_port, 2000, 150);
    if(rc)
       return 0x12;
    rc = lpc.createUDPserverconnection(base_port+1, 2000, 150);
    if(rc)
       return 0x12;
	type = 0;

//??	UdpMsgOut = (unsigned char *) &WriteBuff[0];
	return 0;
}

int SiM_VirtualSetup::InitTCP(int debug_mode)
{  int rc;
    WSADATA wsaData = {0};

	  // Initialize Winsock
    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc != 0) 
	{   printf("WSAStartup failed: %d\n", rc);
        return 1;
    }

	ShowMyIp();
	rc = tcp.createTCPserverconnection(tcp_port, 200000, 2000);
	type = 1;

	return 0;
}

#if USE_COMPORT
extern HANDLE initComPort(int NComPort, int Bitrate, int fw);

int SiM_VirtualSetup::InitCOM(int NComPort, int Bitrate, int debug_mode)
{  int idd;
    uintptr_t hid;
	void *ptr;
    Hcom = initComPort(NComPort, Bitrate, 0);
    if(Hcom == NULL) return 1;
	Ncom = NComPort;
	ptr = this;
    hid = _beginthread(ComWork,0,(void *) ptr);

	type = 2;
  return 0; 
}

#endif
extern void procrec (U8 *buf, U16 len); 

int SiM_VirtualSetup::InitConnection(void)
{   int i, l, istimeout=0;
    int rc, t0, iwc, iwc0=0;
    int  tclockOld, tclock;
    socklen_t addr_len;
extern volatile int tcp_connect_on;

	if(type == -1)
	{	printf("Call first InitUDP/TCP/COM\n");
	    return(-1);
	}
		
if(type == 0) //UDP
{
	l=sizeof(ReadBuff);
	t0 = clock();

    for(i=0; ;i++)
	{    rc = lpcKl.Read(ReadBuff, l);
	     if(rc == -3)
			    continue;
	     if(rc < 0)
			    return -1;
	     if(rc)
		 {   memcpy(WriteBuff,ReadBuff,rc);
			 WriteBuff[6]='O';
	         WriteBuff[7]='K';
			 rc = lpcKl.Send(WriteBuff,8);
			 Nwrite = 0;
			 printf("Client connection from from %s\n", inet_ntoa(lpcKl.client.sin_addr));
			 break;
		 }
		 iwc = (clock()-t0)/1000;
		 if(iwc != iwc0)
	     {   printf("Wait %i\r", iwc); fflush(stdout);
		     iwc0 = iwc;
		 }
		 Sleep(10);
		 SiM();
		 mystuff(1);
	}
	if(rc == 0)
		  return -1;
} else {
	l=sizeof(ReadBuff);
	t0 = clock();

	rc = listen(tcp.sock0,5);
     if (rc == -1)
     { 
#ifdef _WIN32
      printf("listen error = %d\n", WSAGetLastError() );
       closesocket(tcp.sock0);
#else
			 perror("listen");
#endif
                exit(1);
      }
/* Вот тут сидим и ждем, будет запрос на коннект - будет возвращено управление */


while(1)
{			tcp_connect_on = 0;
/* для каждого из дочерних процессов не забыть создать свои экземпляры s1 и csa*/
                addr_len = sizeof (struct sockaddr_in);
				tcp.sock = accept(tcp.sock0, (struct sockaddr *) &tcp.client, &addr_len);
/* В csa будет помещен адрес клиента, который к нам ломится */
                if (tcp.sock == -1)
				{
#ifdef _WIN32
			      printf("accept(), error = %d\n", WSAGetLastError() );
#else
					perror("accept");
#endif
				   closesocket(tcp.sock);
				   exit(1);
				};
				tcp_connect_on = 1;
				tcp.timeLastRW = clock();
/* wait for client */
        tclockOld = GetClock();
        istimeout=0;
     do
     {
	     rc = tcp.Read(ReadBuff, l);
		 if (rc == -3)
		 { //socket error
			 break;
//			 continue;  //??
		 }
	     if(rc < 0)
			    return -1;
		 tclock = GetClock();
		 if(rc == 0)
		 {
//			 if ((tclock - tclockOld) >  tcp.timeout)
		    if ((tclock - tcp.timeLastRW) >  tcp.timeout)
			{   istimeout = 1;
				 break;
			} else {
			 Sleep(10);
			}
		 } else {
			 tclockOld = tclock;
			 procrec ((U8 *)ReadBuff, (U16) rc); 
		 }
	 } while(1);
      closesocket(tcp.sock);
  }

}
	TimerCOn();
	return 0;
}

int SiM_VirtualSetup::SendTCP(unsigned char *buf, int l)
{   int rc;
    rc = tcp.Send((char *)buf,l);
	Nwrite = 0;
    return rc;

}

int SiM_VirtualSetup::Send(void)
{   int rc;
    if(Nwrite <= 0)
		 return 0;
    rc = lpc.Send(WriteBuff,Nwrite);
	Nwrite = 0;
    return rc;
}

int SiM_VirtualSetup::Read(void)
{  int i, rc,l, t0;
static int tlast = -1;
M0:	l=sizeof(ReadBuff);
	Nread = 0;
    rc = lpc.Read(ReadBuff, l,0);
	if(rc < 0)
	{  return -1;
	}
	if(rc == 0)
	{
       if(clock() - tlast > 1000)  /* reconnect ? */
	   {   int rc1;
		    rc1 = lpcKl.Read(ReadBuff, l, 0);
			if(rc1 > 0)
			{   memcpy(WriteBuff,ReadBuff,rc);
			    WriteBuff[6]='O';
	            WriteBuff[7]='K';
			    rc = lpcKl.Send(WriteBuff,8);
			    Nwrite = 0;
			     printf("Client connection from from %s\n", inet_ntoa(lpcKl.client.sin_addr));
				 goto M0;
		    }
	   }
		return 0;
	}
	Nread = rc;
	tlast = clock();
	return rc;
}

int SiM_VirtualSetup::Work(void)
{  int i, rc,l;
	char Buff[1000];
	l=sizeof(ReadBuff);
	for(i=0;i<1000;i++)
	{   // rc = lpcKl.Read(Buff, l);
	    // printf("%i lpcKl rc =%i\n", i,rc);
	     rc = lpc.Read(Buff, l);
	     printf("%i lpc rc =%i\n", i,rc);
		 Sleep(200);
	}
	return 0;
}

extern "C" U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *bf, U16 len); 
extern int Udp_Lsend;

/*
int Log(int vx, int vx1, int vy)
{   static double xx[3] = {0.,0.,0.};
   static int ixx[3] = {0,0,0};

	ixx[0] += vx;
	ixx[1] += vx1;
	ixx[2] += vy;
	xx[0] = ixx[0] * StolSim.Mashtab[0];
	xx[1] = ixx[1] * StolSim.Mashtab[1];
	xx[2] = ixx[2] * StolSim.Mashtab[2];

	fprintf(fp,"%i %i %i | %i %i %i | %f %f %f\n",ixx[0],ixx[1],ixx[2], vx,vx1, vy, xx[0], xx[1], xx[2]);

	return 0;
}
*/
#if USE_COMPORT

void ComWork(void * param)
{  SiM_VirtualSetup * psim;
   psim = (SiM_VirtualSetup *)param;
   psim->COMirqImit();
}

extern int Comgets(HANDLE H_com,char *Buf,int buflen);
extern "C" int AddUART_RX_to_buffer(int nport, char byte);

int SiM_VirtualSetup::COMirqImit(void)
{  int nbytes, i;
	char str[256];
	for(;;)
	{    nbytes = Comgets(Hcom,str,256); //прочитаем, если что есть в буфере
	     if(nbytes)
		 {  for(i=0; i<nbytes; i++)
			{
				AddUART_RX_to_buffer(0,str[i]);
//		        if(  str[i] >= 0x0a ) printf("%c", str[i]);
//				else  printf(".");
/*
			if(detect1)
		    {     if(str[i] == ':') { detect3=1; detect1=0; detectCount1 = 0; detectKey = 0;}
		    }  else   if (detect3) {
				detectCount1++;
				if(detectLen == detectCount1)
				{   printf("!!!"); fflush(stdout);
				    detect3 = 0;
					detectKey = 1;
					detectTime = clock();
				}
		    }
*/

		    }
//		    fflush(stdout);
		 } else {
			 Sleep(1);
		 }
	}
	return 0;
}
#endif //USE_COMPORT
