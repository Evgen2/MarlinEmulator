/* UDPconnection.cpp */
#include "stdafx.h"
#include <WinSock.h>
#include "UDPconnection.hpp"

int ShowMyIp(void);

//создать серверное соединение на порту _port, таймаутами _timeout, _timeoutAnswer
int UDPconnection::createUDPserverconnection(int _port, int _timeout, int _timeoutAnswer)
{   int rc, n;
    const unsigned int nonblocking = TRUE;
    struct sockaddr_in client2;  //локальный порт на клиенте

    memset(&client,0,sizeof(sockaddr_in));
    client.sin_family = AF_INET;//PF_UNIX;
    client.sin_port = INADDR_ANY;

    memset(&server,0,sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr =   INADDR_ANY; //inet_addr(_IpTo); //преобразовывает и записывает IP-адрес
    sock = socket( AF_INET, SOCK_DGRAM,  0 /* IPPROTO_UDP */ ); //создаем сокет
    if(sock == -1)
    {  wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
       printf("Ошибка создания сокета\n");
       return(1);
    }

    rc = bind(sock, (struct sockaddr *)&server, sizeof(sockaddr_in));
    if(rc == -1)
    {  printf("Ошибка вызова bind error = %d\n", WSAGetLastError() );
       closesocket(sock);
       return(2);
    }

    n = sizeof(sockaddr_in);
    rc = getsockname(sock, (struct sockaddr *)&client2, &n);
    printf("Server port: %x\n",  _port);
/* */
    rc = ioctlsocket(sock, FIONBIO, (u_long *)&nonblocking);
    if (rc < 0)
    {  printf("Ошибка вызова ioctl error = %d\n", WSAGetLastError() );
       closesocket(sock);
       return(3);
    }

    port = _port;
    timeout = _timeout;
    timeoutAnswer = _timeoutAnswer;
 
	return 0; 
}

//создать соединение с ip адресом _IpToб портом _port, таймаутами _timeout, _timeoutAnswer
int UDPconnection::createUDPconnection(char _IpTo[], int _port, int _timeout, int _timeoutAnswer)
{   int rc, n;
    const unsigned int nonblocking = TRUE;
    struct sockaddr_in client2;  //локальный порт на клиенте

    memset(&client,0,sizeof(sockaddr_in));
    client.sin_family = AF_INET;//PF_UNIX;
    client.sin_port = INADDR_ANY;

    memset(&server,0,sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr = inet_addr(_IpTo); //преобразовывает и записывает IP-адрес
    sock = socket( AF_INET, SOCK_DGRAM,  0 /* IPPROTO_UDP */ ); //создаем сокет
    if(sock == -1)
    {  wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
       printf("Ошибка создания сокета\n");
       return(1);
    }

    rc = bind(sock, (struct sockaddr *)&client, sizeof(sockaddr_in));
    if(rc == -1)
    {  printf("Ошибка вызова bind error = %d\n", WSAGetLastError() );
       closesocket(sock);
       return(2);
    }

    n = sizeof(sockaddr_in);
    rc = getsockname(sock, (struct sockaddr *)&client2, &n);
    printf("Controller addr: %s  port:%x\n", _IpTo, _port);
/* */
    rc = ioctlsocket(sock, FIONBIO, (u_long *)&nonblocking);
    if (rc < 0)
    {  printf("Ошибка вызова ioctl error = %d\n", WSAGetLastError() );
       closesocket(sock);
       return(3);
    }

    port = _port;
    strcpy(IpTo, _IpTo);
    timeout = _timeout;
    timeoutAnswer = _timeoutAnswer;

 
	return 0; 
}


//int razSend1 = 0;

//послать буфер с подтверждением
//rc = 0 Ok
//rc = 1 Timeout
//rc = 2 прочие ошибки
//rc = 3 плохой сокет
//rc = 4 длина входного буфера неправильная
//rc = 5 длина выходного буфера не равна длине полученного сообщения
int UDPconnection::SendAndConfirm(char bufin[], int len, char bufout[], int lenout)
{  int i, rc,l, nb=0, nRep=0, rc0=0, nRepRoute=0, isTimeout, isTimeout2;
   int razSend = 0, raz;
   unsigned char buff_in[1500];
   unsigned char buff_out[1500];
   int tout0Answer, tout1Answer, dtAnswer;
   int tout0, tout1;
   int start = 0;
static int razSendold=0,tclockOld=0;   

   if(sock <= 0)
        return 3;

   if(len > 1400 || len < 0)
        return 4;
//если что-то есть на - входе - прочитать
M0:
    rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
    if(rc == SOCKET_ERROR)
    {   ierr = WSAGetLastError();
          if(WSAEWOULDBLOCK != ierr)
           {     
                 printf("recv error= %d\n", ierr );
                 rc0 = ierr;
                 stat[3]++;
           }
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
       {  if(clock()-tclockOld < 100)
          {
              Sleep(1);
              tclockOld = clock()-200;
              goto M0;
          }
       }
   }

   razSend = 0;
   isTimeout2 = 1;
      nb = 0;
   tout0 = clock();
   do
   {
      ierr = 0;
      razSend++;
      rc = sendto(sock,bufin,len,0,(struct sockaddr *)&server,sizeof(sockaddr_in));
      if(rc == SOCKET_ERROR)
      {   ierr = WSAGetLastError();
           printf("sendto error= %d\n", ierr );
          
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
     tout0Answer = clock();
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
      {   ierr = WSAGetLastError();
           printf("select( error= %d\n", ierr );
	  }
  rc = -2;
  if(num)
          rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
}
          if(rc < 0)
          {   if(rc == SOCKET_ERROR)
               {   ierr = WSAGetLastError();
                    printf("select( error= %d\n", ierr );
					break;
			   }
                 tout1Answer = clock();
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
           {   if((nb - lenout) != 1) /* временно до пока А. Козлов не поправит */
               { struct Msg1 *pm1, *pm2;
                pm1 = (struct Msg1 *) bufin;
                pm2 = (struct Msg1 *) buff_out;
                  printf("\a\nПолучено: %i байт != %i, cmdR=%i S=%i rS=%i<\n",
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

      tout1 = clock();
   } while(abs(tout1 - tout0) < timeout);

   if( isTimeout2) stat[7]++;
   if(razSend > 2) stat[8]++;

   memcpy(bufout,buff_out,lenout);
   if(rc0 == 0)
   { // if(ierr == SOCEAGAIN)
	   if(isTimeout2)
	   {   rc0 = 1;
//          printf("таймаут после %i попыток\n",nRep);
      }
   }
//   printf("%i %i %i %i raz=%i dt=%i\n", lenout, nb, stat[7],stat[8], raz, clock()-tout0);
   razSendold = razSend;     
   if(rc0 == 0 && nb == 0)
	     printf("hren\n");
  tclockOld = clock();
   return rc0;
}


//послать буфер с подтверждением с переменной длиной ответа
//rc = 0 Ok
//rc = 1 Timeout
//rc = 2 прочие ошибки
//rc = 3 плохой сокет
//rc = 4 длина входного буфера неправильная
//rc = 5 длина выходного буфера не равна длине полученного сообщения
int UDPconnection::SendAndConfirm2(char bufin[], int len, char bufout[], int lenout, int shiftLansw, int size0, int FirstByte)
{  int i, rc,l, nb=0, nRep=0, rc0=0, nRepRoute=0, isTimeout, isTimeout2;
   int razSend = 0, raz;
   unsigned char buff_in[1500];
   unsigned char buff_out[1500];
   int tout0Answer, tout1Answer, dtAnswer,la=0;
   int tout0, tout1, num;
   int start = 0;
   if(sock <= 0)
        return 3;

   if(len > 1400 || len < 0)
        return 4;
//если что-то есть на - входе - прочитать
    rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
    if(rc == SOCKET_ERROR)
    {   ierr = WSAGetLastError();
          if(WSAEWOULDBLOCK != ierr)
           {     
                 printf("recv error= %d\n", ierr );
                 rc0 = ierr;
                 stat[3]++;
           }
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
   tout0 = clock();
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
      {   ierr = WSAGetLastError();
           printf("sendto error= %d\n", ierr );
          
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
      tout0Answer = clock();
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
      {   ierr = WSAGetLastError();
           printf("select( error= %d\n", ierr );
	  }
  rc = -2;
  if(num)
          rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);

}

          if(rc < 0)
          {   if(rc == SOCKET_ERROR)
               {   ierr = WSAGetLastError();
                    printf("select( error= %d\n", ierr );
					break;
			   }
                 tout1Answer = clock();
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
                {  if(FirstByte != buff_out[0]) /* чужой ответ ? */
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
                  printf("\a\n2Получено: %i байт != %i\n",nb, la);
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
      tout1 = clock();
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
//          printf("таймаут после %i попыток\n",nRep);
      }
   }

   if(razSend>1)
      printf("!!2!razSend=%i\n",razSend);
  if(isTimeout2)
      printf("!!2!isTimeout2 nb=%i\n", nb);

   return rc0;
}

int UDPconnection::Read(char bufin[], int len)
{  int rc;
   rc = Read(bufin, len, 5000);
   return rc;
}

int UDPconnection::Read(char bufin[], int len, int timeout_ms)
{  int rc = 0, i;
   int sockList[2],  num=0;
   fd_set fds;
  struct timeval timeout;
   unsigned char buff_out[1500];
   int addr_len;

   FD_ZERO(&fds);
   FD_SET(sock, &fds);
   sockList[0] = sock;
   timeout.tv_sec = 0;
   timeout.tv_usec = timeout_ms;
   num = select(sizeof(fds)*8, &fds, NULL, NULL, &timeout);
   if(num == SOCKET_ERROR)
   {   ierr = WSAGetLastError();
           printf("select( error= %d\n", ierr );
       return -2;
   }
   if(num == 0)
	   return 0;

    addr_len = sizeof(struct sockaddr);

//    rc = recv(sock, (char *)&buff_out,sizeof(buff_out),0);
    rc = recvfrom(sock, (char *)&buff_out, sizeof(buff_out), 0, (struct sockaddr *)&client, &addr_len);
    if(rc < 0)
    {   if(rc == SOCKET_ERROR)
         {   ierr = WSAGetLastError();
              printf("select( error= %d\n", ierr );
			  return -3;
	     }
         printf("recv rc= %d\n", rc);
		 return -4;
	}

	for(i=0; i<rc; i++)
	{   bufin[i] = buff_out[i];
	}
    return rc;
}


int UDPconnection::Send(char buf[], int len)
{  int rc;
    rc = sendto(sock,buf,len,0,(struct sockaddr *)&client,sizeof(sockaddr_in));
    if(rc == SOCKET_ERROR)
    {    ierr = WSAGetLastError();
         printf("sendto error= %d\n", ierr );
         return -1;
    }
	return rc;
}

