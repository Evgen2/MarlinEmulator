/* TCPconnection.cpp */
#include "stdafx.h"

#include "TCPconnection.hpp"

int ShowMyIp(void);
int InitTCPIP(void);

//������� ��������� ���������� � ip ������� _IpTo� ������ _port, ���������� _timeout, _timeoutAnswer
int TCPconnection::createTCPserverconnection(int _port, int _timeout, int _timeoutAnswer)
{   int rc;
    socklen_t n;
#ifdef _WIN32
    const unsigned int nonblocking = TRUE;
#endif
    struct sockaddr_in client2;  //��������� ���� �� �������

    memset(&client,0,sizeof(sockaddr_in));
    client.sin_family = AF_INET;//PF_UNIX;
    client.sin_port = INADDR_ANY;

    memset(&server,0,sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr =   INADDR_ANY; //inet_addr(_IpTo); //��������������� � ���������� IP-�����
    sock0 = socket( AF_INET, SOCK_STREAM,  0 /* SOCK_DGRAM = UDP SOCK_STREAM = TCP */ ); //������� �����
    if(sock0 == -1)
    {
#ifdef _WIN32
        printf("socket function failed with error = %d\n", WSAGetLastError() );
#else
        perror("socket failed");
#endif // _WIN32
       printf("������ �������� ������\n");
       return(1);
    }

    rc = bind(sock0, (struct sockaddr *)&server, sizeof(sockaddr_in));
    if(rc == -1)
    {
#ifdef _WIN32
        printf("������ ������ bind error = %d\n", WSAGetLastError() );
#else
        perror("bind");
#endif //_WIN32
       closesocket(sock0);
       return(2);
    }

    n = sizeof(sockaddr_in);
    rc = getsockname(sock0, (struct sockaddr *)&client2, &n);
    printf("TCP Server port: %x\n",  _port);
#if 0
/* nonblocking mode */
#ifdef _WIN32
    rc = ioctlsocket(sock, FIONBIO, (u_long *)&nonblocking);
    if (rc < 0)
    {  printf("������ ������ ioctl error = %d\n", WSAGetLastError() );
       closesocket(sock);
       return(3);
    }
#else
    rc = fcntl(sock, F_SETFL, O_NONBLOCK);
    if(rc == -1)
    {   perror("fcntl");
       closesocket(sock);
       return(3);
    }
#endif // WIN32
#endif //0
    port = _port;
    timeout = _timeout;
    timeoutAnswer = _timeoutAnswer;
	timeLastRW = clock();
	return 0;
}

//������� ���������� � ip ������� _IpTo� ������ _port, ���������� _timeout, _timeoutAnswer
int TCPconnection::createTCPconnection(char _IpTo[], int _port, int _timeout, int _timeoutAnswer)
{   int rc;
    socklen_t n;
#ifdef _WIN32
    const unsigned int nonblocking = TRUE;
#endif
    struct sockaddr_in client2;  //��������� ���� �� �������

    memset(&client,0,sizeof(sockaddr_in));
    client.sin_family = AF_INET;//PF_UNIX;
    client.sin_port = INADDR_ANY;

    memset(&server,0,sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr = inet_addr(_IpTo); //��������������� � ���������� IP-�����
    sock = socket( AF_INET, SOCK_STREAM,  0 /* SOCK_DGRAM = UDP SOCK_STREAM = TCP */ ); //������� �����
    if(sock == -1)
    {
#ifdef _WIN32
        printf("socket function failed with error = %d\n", WSAGetLastError() );
#else
        perror("socket failed");
#endif // _WIN32
       printf("������ �������� ������\n");
       return(1);
    }

	printf("Try connect to TCP %s port %i\n", _IpTo, _port);
   rc = connect(sock, (struct sockaddr *) &server, sizeof (struct sockaddr_in));
  if (rc==(-1))
  {  
#ifdef _WIN32
//todo �������� �������� �� ������ WSAECONNREFUSED
	  int errcode;
	  errcode = WSAGetLastError();
	  if(errcode ==  WSAETIMEDOUT)
        printf("connect  to %s port %i timeout\n",_IpTo, _port );
	  else
	    printf("Connect to %s port %i failed with error = %d\n", _IpTo, _port, errcode);
#else
	     perror("connect");
	    printf("(Try connect to %s port %i)\n", _IpTo, _port);
#endif
       printf("Cann't establish TCP connection\r\n");
       closesocket(sock);
       return 2;
   };
printf("TCP connection established !!!\r\n");

    n = sizeof(sockaddr_in);
    rc = getsockname(sock, (struct sockaddr *)&client2, &n);
    printf("Controller addr: %s  port:%x\n", _IpTo, _port);

/* nonblocking mode */
#ifdef _WIN32
    rc = ioctlsocket(sock, FIONBIO, (u_long *)&nonblocking);
    if (rc < 0)
    {  printf("������ ������ ioctl error = %d\n", WSAGetLastError() );
       closesocket(sock);
       return(3);
    }
#else
    rc = fcntl(sock, F_SETFL, O_NONBLOCK);
    if(rc == -1)
    {   perror("fcntl");
       closesocket(sock);
       return(3);
    }
#endif // WIN32

    port = _port;
    strcpy(IpTo, _IpTo);
    timeout = _timeout;
    timeoutAnswer = _timeoutAnswer;
	timeLastRW = clock();

	return 0;
}

int ShowMyIp(void)
{
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
    {
#ifdef _WIN32
		printf("Error when getting local host name= %d\n", WSAGetLastError() );
#else
		perror("Error when getting local host name\n" );
#endif
        return 1;
    }
    printf("Host name is %s\n", ac);

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0)
    {
#ifdef _WIN32
		printf("Yow! Bad host lookup. error= %d\n", WSAGetLastError() );
#else
		herror("Yow! Bad host lookup.");
#endif
        return 1;
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i)
	{
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("My address  %i : %s\n", i, inet_ntoa(addr));
    }

    return 0;
}

//int razSend1 = 0;

//������� ����� � ��������������
//rc = 0 Ok
//rc = 1 Timeout
//rc = 2 ������ ������
//rc = 3 ������ �����
//rc = 4 ����� �������� ������ ������������
//rc = 5 ����� ��������� ������ �� ����� ����� ����������� ���������
int TCPconnection::SendAndConfirm(char bufin[], int len, char bufout[], int lenout)
{  int i, rc, nb=0, nRep=0, rc0=0, nRepRoute=0, isTimeout, isTimeout2;
   int razSend = 0, raz;
   unsigned char buff_out[1500];
   int tout0Answer, tout1Answer;
   int tout0, tout1;
   int start = 0;
static int razSendold=0,tclockOld=0;

   if(sock <= 0)
        return 3;

   if(len > 1400 || len < 0)
        return 4;
//���� ���-�� ���� �� - ����� - ���������
M0:
    rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
    if(rc == SOCKET_ERROR)
    {
#ifdef _WIN32
          ierr = WSAGetLastError();
          if(WSAEWOULDBLOCK != ierr)
           {
                 printf("recv error= %d\n", ierr );
                 rc0 = ierr;
                 stat[3]++;
           }
#else
        perror("recv");
#endif // _WIN32          ierr = WSAGetLastError();
           stat[4]++;
    } else {
          nb = rc;
          if(rc > 0)
          { nresv ++;
            nBresv += nb;
            stat[5]++;
          }
	}

   if(razSendold > 1)
   {   if(nb == 0)
       {  if(GetClock()-tclockOld < 100)
          {
              Sleep(1);
              tclockOld = GetClock()-200;
              goto M0;
          }
       }
   }

   razSend = 0;
   isTimeout2 = 1;
      nb = 0;
   tout0 = GetClock();
   do
   {
      ierr = 0;
      razSend++;
      rc = sendto(sock,bufin,len,0,(struct sockaddr *)&server,sizeof(sockaddr_in));
      if(rc == SOCKET_ERROR)
      {
#ifdef _WIN32
          ierr = WSAGetLastError();
           printf("sendto error= %d\n", ierr );
#else
         perror("sendto");
#endif // _WIN32
         rc0 = ierr;
         stat[1]++;
         break;
      } else {
         nBsend += rc;
         if(start)
         {  nResend++;
            nRep++;
         }
         start = 1;
         stat[0]++;
      }
     tout0Answer = GetClock();
      isTimeout = 1;

      for(raz=0; ;raz++)
      {
          ierr = 0;
{ int sockList[2], timeout_ms=5000, num=0;
  fd_set fds;
  struct timeval timeout;

   FD_ZERO(&fds);
   FD_SET(sock, &fds);
  sockList[0] = sock;
  timeout.tv_sec = 0;
  if(lenout > 60)
  {  timeout_ms = 8000;
     if(lenout > 120)
         timeout_ms = 16000;
  }
  timeout.tv_usec = timeout_ms;
  num = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);
      if(num == SOCKET_ERROR)
      {
#ifdef _WIN32
          ierr = WSAGetLastError();
           printf("select( error= %d\n", ierr );
#else
           perror("select");
#endif // _WIN32
	  }
  rc = -2;
  if(num)
          rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
}
          if(rc < 0)
          {   if(rc == SOCKET_ERROR)
               {
#ifdef _WIN32
                    ierr = WSAGetLastError();
                    printf("recv error= %d\n", ierr );
#else
                    perror("recv");
#endif // _WIN32

					break;
			   }
                 tout1Answer = GetClock();
                 if(abs(tout1Answer - tout0Answer) < timeoutAnswer)
                 {  //  if(raz > 256*64)  //26Kbytes
                                if(raz & 0x01)
                                           Sleep(1);
                 } else
                      break;

             stat[4]++;
          } else {
            if(rc > 0)
            {  nb += rc;
               nresv ++;
               nBresv += rc;
               stat[2]++;
               isTimeout = 0;
			   break;
             } else
                 break;
          }
      } //endof for(raz=0; ;raz++)

      if(isTimeout)
      {  stat[6]++;
	     if(nb == 0)
			   break;
      }

      if(ierr)
      {   //?? if(SOCEAGAIN != ierr)
           {  isTimeout2 = 0;
              break;
           }
      } else {
           if(nb != lenout)
           {   if((nb - lenout) != 1) /* �������� �� ���� �. ������ �� �������� */
               { struct Msg1 *pm1, *pm2;
                pm1 = (struct Msg1 *) bufin;
                pm2 = (struct Msg1 *) buff_out;
                  printf("\a\n��������: %i ���� != %i, cmdR=%i S=%i rS=%i<\n",
                    nb, lenout, pm2->cmd, pm1->cmd, razSendold);
                 stat[5]++;
                 rc0 = 5;
                 isTimeout2 = 0;
                 for(i=0; i < lenout; i++)  printf("%2x ",bufin[i]);
                 printf("\n>");
                  for(i=0; i < nb; i++) printf("%2x ",buff_out[i]);
                 printf("\n");
                 return 5;
             }
                break;
           } else {
                isTimeout2 = 0;
               break;
           }

      }

      tout1 = GetClock();
   } while(abs(tout1 - tout0) < timeout);

   if( isTimeout2) stat[7]++;
   if(razSend > 2) stat[8]++;

   memcpy(bufout,buff_out,lenout);
   if(rc0 == 0)
   { // if(ierr == SOCEAGAIN)
	   if(isTimeout2)
	   {   rc0 = 1;
//          printf("������� ����� %i �������\n",nRep);
      }
   }
//   printf("%i %i %i %i raz=%i dt=%i\n", lenout, nb, stat[7],stat[8], raz, clock()-tout0);
   razSendold = razSend;
   if(rc0 == 0 && nb == 0)
	     printf("hren\n");
  tclockOld = GetClock();
   return rc0;
}


//������� ����� � �������������� � ���������� ������ ������
//rc = 0 Ok
//rc = 1 Timeout
//rc = 2 ������ ������
//rc = 3 ������ �����
//rc = 4 ����� �������� ������ ������������
//rc = 5 ����� ��������� ������ �� ����� ����� ����������� ���������
int TCPconnection::SendAndConfirm2(char bufin[], int len, char bufout[], int lenout, int shiftLansw, int size0, int FirstByte)
{  int i, rc, nb=0, nRep=0, rc0=0, nRepRoute=0, isTimeout, isTimeout2;
   int razSend = 0, raz;
   unsigned char buff_out[1500];
   int tout0Answer, tout1Answer,la=0;
   int tout0, tout1, num;
   int start = 0;
   if(sock <= 0)
        return 3;

   if(len > 1400 || len < 0)
        return 4;
//���� ���-�� ���� �� - ����� - ���������
    rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
    if(rc == SOCKET_ERROR)
    {
#ifdef _WIN32
          ierr = WSAGetLastError();
          if(WSAEWOULDBLOCK != ierr)
           {
                 printf("recv error= %d\n", ierr );
                 rc0 = ierr;
                 stat[3]++;
           }
#else
          perror("recv");
#endif // _WIN32

           stat[4]++;
    } else {
          if(rc > 0)
          {
            nb = rc;
            nresv ++;
            nBresv += nb;
            stat[5]++;
          }
   }

//   ncmd = (ncmd + 2) & 0xff;
 //   razSend1 = razSend;
   razSend = 0;
   isTimeout2 = 1;
   tout0 = GetClock();
      nb = 0;
   do
   {
//    buff_in[0] = ncmd;
//      memcpy(&buff_in[1],bufin, len);
//      l = len+1;
      ierr = 0;
      razSend++;
      rc = sendto(sock,bufin,len,0,(struct sockaddr *)&server,sizeof(sockaddr_in));
      if(rc == SOCKET_ERROR)
      {

#ifdef _WIN32
          ierr = WSAGetLastError();
           printf("sendto error= %d\n", ierr );
#else
         perror("sendto");
#endif // _WIN32

         rc0 = ierr;
         stat[1]++;
         break;
      } else {
         nBsend += rc;
         if(start)
         {  nResend++;
            nRep++;
         }
         start = 1;
         stat[0]++;
      }
      tout0Answer = GetClock();
      isTimeout = 1;
/*******************************/
      for(raz=0; ;raz++)
      {
          ierr = 0;
{ int sockList[2], timeout_ms=5000, num=0;
  fd_set fds;
  struct timeval timeout;

   FD_ZERO(&fds);
   FD_SET(sock, &fds);
  sockList[0] = sock;
  timeout.tv_sec = 0;
  if(lenout > 60)
  {  timeout_ms = 8000;
     if(lenout > 120)
         timeout_ms = 16000;
  }
  timeout.tv_usec = timeout_ms;
  num = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);

      if(num == SOCKET_ERROR)
      {
#ifdef _WIN32
          ierr = WSAGetLastError();
           printf("select( error= %d\n", ierr );
#else
           perror("select");
#endif // _WIN32
	  }
  rc = -2;
  if(num)
          rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);

}

          if(rc < 0)
          {   if(rc == SOCKET_ERROR)
               {
#ifdef _WIN32
          ierr = WSAGetLastError();
          printf("recv error= %d\n", ierr );
#else
          perror("recv");
#endif // _WIN32

					break;
			   }
                 tout1Answer = GetClock();
                 if(abs(tout1Answer - tout0Answer) < timeoutAnswer)
                 {  //  if(raz > 256*64)  //26Kbytes
                                if(raz & 0x01)
                                           Sleep(1);
                 } else
                      break;

             stat[4]++;
          } else {
            if(rc > 0)
            {  nresv ++;
               nBresv += rc;
               stat[2]++;
               isTimeout = 0;
                if(nb == 0)
                {  if(FirstByte != buff_out[0]) /* ����� ����� ? */
                   {   isTimeout = 1;
                       continue;
                   }

                    la = (int) *((short int *) &buff_out[shiftLansw]);
                    la = la * size0 + shiftLansw + 2;
                    if(la < 0 || la > 1500) la = 0;
                    if(la >lenout)
                            la = lenout;
                }
                nb += rc;
                if(nb >= la)
                   break;

			   break;
             } else
                 break;
          }
          Sleep(1);
      } //endof for(raz=0; ;raz++)

/*******************************/

      if(isTimeout)
      {  stat[6]++;
	     if(nb == 0)
			   break;
      }

      if(ierr)
      {   // if(SOCEAGAIN != ierr)
           {  isTimeout2 = 0;
              break;
           }
      } else {
           if(nb != la)
           {
//               la = (int) *((short int *) &buff_out[shiftLansw]);
//               la = la * size0 + shiftLansw + 2;

//               if(la != nb)
//               {
                  printf("\a\n2��������: %i ���� != %i\n",nb, la);
                 stat[5]++;
                 rc0 = 5;
                 isTimeout2 = 0;
                 for(i=0; i < lenout; i++)  printf("%2x ",bufin[i]);
                 printf("\n>");
                  for(i=0; i < nb; i++) printf("%2x ",buff_out[i]);
                 printf("\n");
                 return 5;
//             }
                isTimeout2 = 0;
                break;
//todo!!!
//           if(buff[0] != ncmd)
           } else {
//                for(i=0; i < rc; i++) printf("%2x ",buff_out[i]);
///                printf("\n");
                isTimeout2 = 0;
               break;
           }

      }
      tout1 = GetClock();
printf("raz=%i rc=%i num=%i razSend=%i nb=%i dt=%i %i %i\n", raz, rc, num, razSend, nb,
     tout1 - tout0Answer, tout1-tout0,  timeoutAnswer);

   } while(abs(tout1 - tout0) < timeout);

   if( isTimeout2) stat[7]++;
   if(razSend > 2) stat[8]++;

   if(nb > 0)
   {  if(nb > lenout) nb = lenout;
           memcpy(bufout,buff_out,nb);
   }
   if(rc0 == 0)
   { // if(ierr == SOCEAGAIN)
	  if(isTimeout2)
      {   rc0 = 1;
//          printf("������� ����� %i �������\n",nRep);
      }
   }

   if(razSend>1)
      printf("!!2!razSend=%i\n",razSend);
  if(isTimeout2)
      printf("!!2!isTimeout2 nb=%i\n", nb);

   return rc0;
}

int TCPconnection::Read(char bufin[], int len)
{  int rc;
   rc = Read(bufin, len, 5000);
   return rc;
}

int TCPconnection::Read(char bufin[], int len, int timeout_ms)
{  int rc = 0, i;
   int sockList[2],  num=0;
   fd_set fds;
  struct timeval timeout;
   unsigned char buff_out[1500];
   socklen_t addr_len;

   FD_ZERO(&fds);
   FD_SET(sock, &fds);
   sockList[0] = sock;
   timeout.tv_sec = 0;
   timeout.tv_usec = timeout_ms;
   num = select(sizeof(fds)*1, &fds, NULL, NULL, &timeout);
   if(num == SOCKET_ERROR)
   {
#ifdef _WIN32
       ierr = WSAGetLastError();
       printf("select( error= %d\n", ierr );
#else
       perror("select");
#endif // _WIN32
       return -2;
   }
   if(num == 0)
	   return 0;

    addr_len = sizeof(struct sockaddr);

//    rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
    rc = recvfrom(sock, (char *)&buff_out, sizeof(buff_out), 0, (struct sockaddr *)&client, &addr_len);
    if(rc < 0)
    {   if(rc == SOCKET_ERROR)
         {
#ifdef _WIN32
              ierr = WSAGetLastError();
              printf("select( error= %d\n", ierr );
#else
             perror("recvfrom");
#endif //_WIN32
			  return -3;
	     }
         printf("recv rc= %d\n", rc);
		 return -4;
	}

	if(rc > 0)
		timeLastRW = clock();

	for(i=0; i<rc; i++)
	{   bufin[i] = buff_out[i];
	}
    return rc;
}


int TCPconnection::Send(char buf[], int len)
{  int rc;
    rc = sendto(sock,buf,len,0,(struct sockaddr *)&client,sizeof(sockaddr_in));
    if(rc == SOCKET_ERROR)
    {
#ifdef _WIN32
         ierr = WSAGetLastError();
         printf("sendto error= %d\n", ierr );
#else
         perror("sendto");
#endif // _WIN32
         return -1;
    }
	if (rc > 0)
		timeLastRW = clock();
	return rc;
}

int InitTCPIP(void)
{
#ifdef _WIN32
    WSADATA wsaData = {0};
	int rc;

  // Initialize Winsock
    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc != 0)
	{   printf("WSAStartup failed: %d\n", rc);
        return 1;
    }
#else
	;
#endif
    GetClockInit();
	return 0;
}

#ifdef _WIN32

static int d_time;
static int d_time_start;

int GetClockInit(void)
{ d_time_start = clock();
  return d_time_start; 
}

int GetClock(void)
{  d_time = clock();
   return  d_time -  d_time_start; 
}

#else

static double d_time;
static double d_time_start;

int GetClockInit(void)
{ struct timespec time;
   clock_gettime(CLOCK_MONOTONIC,&time);
   d_time_start = ((double)time.tv_sec + 1.0e-9 * time.tv_nsec);
   return 0;
}

int GetClock(void)
{   struct timespec time;
     clock_gettime(CLOCK_MONOTONIC,&time);
     d_time = ((double)time.tv_sec + 1.0e-9 * time.tv_nsec);
	 return (int)((d_time - d_time_start) * 1000);
}

#endif
