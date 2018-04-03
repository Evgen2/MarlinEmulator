/* Master_tcp.cpp */
#ifndef WIN32
#include <Net_Config.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "LPC17xx.h"		// Keil: Register definition file for LPC17xx
#ifndef WIN32
#include <RTL.h>
#endif
#include "Master.h"
#include "mybuffer.hpp"

extern "C" void TcpSend(void);
extern "C" U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par);
extern "C" U16 tcp1_callback (U8 soc, U8 evt, U8 *ptr, U16 par);
void send_data_tcp (void); 
void send_data_tcp1 (void);
void send_tcp(char *buf, int len);
extern void on_get_tcp(char *buf, int len);
extern void on_get_tcp_init(void);

void send_tcp(char *buf, int len); 

U8 socket_tcp;
U8 socket_tcp1;
int TcpLsend=0;
//int Tcp1Lsend=0;
volatile int tcp_connect_on =0;
extern int UdpLsend;
extern int UdpOn;	


PACKED unsigned char *TcpMsgOut=NULL;
//PACKED unsigned char *Tcp1MsgOut=NULL;

/*****************************************************/
#ifdef WIN32

extern unsigned char * udp_get_buf(int Nsend);
extern unsigned char * tcp_get_buf(int Nsend);
//char stol_4::BiosDate[12]=__DATE__;     /* дата компиляции биоса */

#endif
/*****************************************************/


/*--------------------------- TCP socket ------------------------------------*/
#ifndef WIN32
void TcpSend(void)
{  if (TcpLsend > 0 /* || Tcp1Lsend > 0 */) {
       send_data_tcp(); 
      TcpLsend = 0;	
	 }
}
#endif
/*--------------------------- Process received data  ------------------------*/
void LedOn(int onOff);

int tstst=0;
void procrec (U8 *buf, U16 len) 
{
  tstst = (tstst+1)%2;
  LedOn(tstst);
	UdpOn = 0;	
	on_get_tcp((char *)buf, len);
  
}

extern "C" void net_stuff(void);

/*--------------------------- TCP send --------------------------------------*/
void send_tcp(char *buf, int len)
{
	if(buf[0] == 'o' && buf[1] == 'k' && len > 4)
			LedOn(tstst);

	if(TcpLsend)
		tstst++;
	if(len > 1460) 
		len = 1460;
	TcpMsgOut = tcp_get_buf(len);
	   if(TcpMsgOut)
		 {
				memcpy(TcpMsgOut, buf, len);
				TcpLsend = len;
			 net_stuff();
		 }
}

//		 TcpLsend = 0;
//     TcpMsgOut = tcp_get_buf(SENDLEN);
void send_data_tcp (void) 
{
  U8 *sendbuf;

  /* TCP */
if(TcpLsend > 0	&& socket_tcp != 0) {
    /* Start Connection */
//    p2 = p2val;
#if 0
    switch (tcp_get_state(socket_tcp)) {
      case TCP_STATE_FREE:
      case TCP_STATE_CLOSED:
//        tcp_connect (socket_tcp, Rem_IP, PORT_NUM, 0);
        break;
      case TCP_STATE_CONNECT:
        if (tcp_check_send (socket_tcp) == __TRUE) {
          tcp_send (socket_tcp, TcpMsgOut, TcpLsend);
					TcpLsend = 0;
        }
        break;
    }
#endif //0
  }
#if 0
	if(Tcp1Lsend > 0	&& socket_tcp1 != 0) {
    /* Start Connection */
//    p2 = p2val;
    switch (tcp_get_state(socket_tcp1)) {
      case TCP_STATE_FREE:
      case TCP_STATE_CLOSED:
//        tcp_connect (socket_tcp, Rem_IP, PORT_NUM, 0);
        break;
      case TCP_STATE_CONNECT:
        if (tcp_check_send (socket_tcp1) == __TRUE) {
          tcp_send (socket_tcp1, Tcp1MsgOut, Tcp1Lsend);
					Tcp1Lsend = 0;
        }
        break;
    }
  }
#endif //0	
}

/* general Internal interface */
U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par) {
  /* This function is called by the TCP module on TCP event */
  /* Check the 'Net_Config.h' for possible events.          */
  par = par;

  if (soc != socket_tcp) {
    return (0);
  }

#if 0
  switch (evt) {
    case TCP_EVT_DATA:
      /* TCP data frame has arrived, data is located at *par1, */
      /* data length is par2. Allocate buffer to send reply.   */
      procrec(ptr, par);
      break;

    case TCP_EVT_CONREQ:
      /* Remote peer requested connect, accept it */
      return (1);

    case TCP_EVT_CONNECT:
      /* The TCP socket is connected */
      on_get_tcp_init();
			tcp_connect_on = 1;
      return (1);
		
		case TCP_EVT_ABORT:
			tcp_connect_on = 0;
      return (1);
  }
#endif //0
  return (0);
}

/* Internal debugging interface */
U16 tcp1_callback (U8 soc, U8 evt, U8 *ptr, U16 par) {
  /* This function is called by the TCP module on TCP event */
  /* Check the 'Net_Config.h' for possible events.          */
  par = par;

  if (soc != socket_tcp) {
    return (0);
  }
#if 0
  switch (evt) {
    case TCP_EVT_DATA:
      /* TCP data frame has arrived, data is located at *par1, */
      /* data length is par2. Allocate buffer to send reply.   */
      procrec(ptr, par);
      break;

    case TCP_EVT_CONREQ:
      /* Remote peer requested connect, accept it */
      return (1);

    case TCP_EVT_CONNECT:
      /* The TCP socket is connected */
      return (1);
  }
#endif 
  return (0);
}


/////////////////////////////////////////////////////////////////////////////
