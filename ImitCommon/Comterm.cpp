 /* Comterm.cpp */
 /* пpимеp pаботы с компоpтами: теpминал */
 /* (с) Евгений Коцюба                   */

#include "stdafx.h"
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>

static HANDLE Hcom;

void SetColor(int text, int background)
{   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

#define FATAL_COLOR         {SetColor(12,0);}
#define WARNING_COLOR       {SetColor(14,0);}
#define NORMAL_COLOR        {SetColor(15,0);}
#define ATTENSION_COLOR     {SetColor(13,0);}

//char Buf[255];
//	DWORD LenBuf;
DCB DCB1;

/* теpминал */
int ComControl(int NComPort, int Bitrate, int fw, int logmode);
HANDLE initComPort(int NComPort, int Bitrate, int fw);
int Comgets(HANDLE H_com,char *Buf,int buflen);

char LogFileName[]="Esp866.log";

FILE *fpLog = NULL; 
int echomode  = 0;


 /***********************/
 /* отладочный теpминал */
int ComControl(int NComPort, int Bitrate, int fw, int logmode)
{  int rc,rc1, X=0,Y=0,ZadX,Cpos,kon,t0=0,t1, isKey=0, hexmode=0;
    short int sts;
/*    int Bitrate=9600; */
    int i;
    int nbytes,ComBuffSize, ierr;
    long int nbytesRead,nbytesWrite;
    int Nunit=0,inCache;
    HANDLE H_com;
    char str[520];

//    sscanf(str,"%i",&NComPort);
    H_com = initComPort(NComPort, Bitrate, fw);
    if(H_com == NULL) return 0;

    if(logmode)    
	{	fpLog = fopen(LogFileName,"wb");
		if(fpLog)
	    {  printf("Logfile=%s\n", LogFileName);
	    }
	} else {
		fpLog = NULL;
	}

M0:
 
    nbytes = Comgets(H_com,str,512);
 
    if(nbytes > 0)
     {  static int total=0, t0=0, t,t1, t10=0;  
        int needflush=0;    
          str[nbytes+1] = 0;
          if(hexmode)
          {    for(i=0; str[i]; i++)  printf("%3x ",str[i]); 
          } else {
             if(str[nbytes-1] == '\r') 
             {   str[nbytes] = '\n';
                 needflush++;
                 nbytes++;
             }     
             printf("%s",str);
/*             
              for(i=0; str[i]; i++)  printf("%3x ",str[i]); 
            { int xx; 
              xx = '\n';
             printf("%x}\n",xx);
             }
*/             
          } 
          if(needflush)
          {   fflush(stdout);
              needflush = 0;
          }
          if(fpLog)
             fwrite(str,1, nbytes, fpLog);
     } 

    t0 = clock();
    for(;;)
    { static int iii=0, rr;
       isKey = 0;
	   rr = _kbhit();
      if(rr)
      {   rc = _getch();
          if(_kbhit())
          {  rc1 = _getch();
            if(rc1){ rc += 100; isKey = 1; }
          }
//	    printf("rc=%i\n",rc);
//          printf("%i %i<\n",rc1, rc); fflush(stdout);   
          
  if(echomode)          
  {
     if(isKey)          
      {  //printf("%c",rc-100); 
      } else {  
        printf("%c",rc); 
      }   
         fflush(stdout);
  }

//          if(isKey) printf("\n");
//          else      printf("%c",rc); 
//          if(rc == '\r')  printf("\n"); 
//          fflush(stdout);
        break;
       }
      t1 = clock();
      if(t1-t0 > 2)
      {  rc =0; break;
      } else {
          Sleep(1); 
	  }
	}

   if(!isKey)
   { 
        if(rc)
        { OVERLAPPED Overlap; 
		  DWORD  numbytes_ok; 
		  if(rc == '\r')
		  {
           str[0]= rc;
           str[1]= '\n';
           str[2]= 0;
// printf("Write %x\n", rc);
		   rc =  WriteFile(H_com, str, 2, &numbytes_ok, NULL); 
		  } else {
           str[0]= rc;
           str[1]= 0;
// printf("Write %x\n", rc);
		   rc =  WriteFile(H_com, str, 1, &numbytes_ok, NULL); 
		  }
   if (rc == 0) //error close port
   { ierr = GetLastError();
		printf("WriteFile Error: %i\n",ierr);
//printf("Hcom=%x\n", Hcom);  
   }
//	    printf("rcW=%i %i\n",rc,numbytes_ok);
 //        rc=  DosWrite(Hcom,str,1,&nbytesWrite);
//  printf("DosWrite rc=%i\n",rc);
         
//             DosWriteIMM(Hcom,str);
        }
   } else {
//  printf("rc=%i##\n",rc);
   } 
   
 goto M0;

   rc = CloseHandle(H_com);
   if (rc == 0) //error close port
   { ierr = GetLastError();
		printf("Open port Error: %i\n",ierr);
		exit(1);
  }

   return 0;
} 


HANDLE initComPort(int NComPort, int Bitrate, int fw)
{   char devName[10];
    int ierr, rc;
  HANDLE hf=NULL;          /* File handle for the device           */

  USHORT usBPS = 2400;/* Bit rate to set the COM port to      */
  ULONG  ulParmLen= 2;/* Maximum size of the parameter packet */
  ULONG  ulAction;    /* Action taken by DosOpen              */
  UCHAR  ucStatus;    /* The COM port status byte             */
  ULONG  ulStatusLen; /* Length of status (the data packet)   */
  UCHAR  Linectrl[3]; /* array for DataBits,Parity,StopBits   */
  CHAR COMSETTING[256]; //="Com1: baud=1200 parity=N data=8 stop=1";

//  LPCTSTR COMSETTING="Com1: baud=1200 parity=N data=8 stop=1";
  if(NComPort < 10)
  {	 sprintf(devName,"COM%i:",NComPort);
     sprintf(COMSETTING,"COM%i: baud=%i parity=N data=8 stop=1", NComPort,Bitrate);
  } else {
		sprintf(devName,"\\\\.\\COM%i",NComPort);
        sprintf(COMSETTING,"baud=%i parity=N data=8 stop=1", Bitrate);
  }

//LPCTSTR COMSETTING[256]; //="Com1: baud=1200 parity=N data=8 stop=1";

  hf = CreateFile(devName, GENERIC_READ|GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL,NULL);           

  if (hf==INVALID_HANDLE_VALUE)
  { 	ierr = GetLastError();
		printf("Open port Error: %i\n",ierr);
		if(ierr == 2)
		      printf("5 = ERROR_FILE_NOT_FOUND\n");
		else if(ierr == 5)
		      printf("5 = ERROR_ACCESS_DENIED\n");
		return NULL;
  } else { 
      printf("Open of %s Ok\n",devName);
  }

/*
The string must have the same form as the mode command's command-line arguments:
COMx[:][baud=b][parity=p][data=d][stop=s][to={on|off}][xon={on|off}][odsr={on|off}][octs={on|off}][dtr={on|off|hs}][rts={on|off|hs|tg}][idsr={on|off}]
*/
    rc = BuildCommDCB(COMSETTING,&DCB1);
	if (rc == 0)	//error DCB
    { ierr = GetLastError();
	 	printf("DCB Structure ERROR: %i\n",ierr);
		exit(1);
   }
   rc = SetCommState(hf,&DCB1);
  if (rc == 0)	//error SetCom
    { ierr = GetLastError();
	 	printf("SetComm Function ERROR: %i\n",ierr);
		exit(1);
   }
  printf("Hcom=%x\n", hf);
  Hcom = hf;
  return hf;
} 


int Comgets(HANDLE H_com,char *Buf,int buflen)
 {   int rc,i,j,is,n, ns, ierr;
     int nbytes,ComBuffSize,nbytesRead,nbytesWrite;
     USHORT  buff[2];
     USHORT  buffOut[2];
	 DWORD numbytes, numbytes_ok, temp; 
     int sts;
     unsigned long int dl;
     COMSTAT ComState;
     OVERLAPPED Overlap;
    dl = 2;
/* */
	rc = ClearCommError(H_com, &temp, &ComState); 
  if (rc == 0)	//error SetCom
    { ierr = GetLastError();
	 	printf("ClearCommError Function ERROR: %i\n",ierr);
		exit(1);
   }
  nbytesRead = 0;
  nbytes = ComState.cbInQue;
  if(nbytes)
  {   if(nbytes >= buflen)
			nbytes = buflen - 1;
	  rc = ReadFile(H_com, Buf, nbytes, &numbytes_ok,  NULL);
   if (rc == 0) //error close port
   { ierr = GetLastError();
		printf("ReadFile Error: %i\n",ierr);
 printf("Hcom=%x\n", H_com);  
   }

	  nbytesRead =  numbytes_ok;
      Buf[nbytesRead] = 0;
  }
#if 0
//   printf("ComgetsIn\n"); fflush(stdout); 
     rc = DosDevIOCtl(
      H_com,IOCTL_ASYNC,    /* Serial-device control      */
      ASYNC_GETCOMMSTATUS, /* Get the COM status byte    */
      NULL, 0, NULL,       /* Length of parameter packet */
      (PULONG) &buff[0],      /* Data packet                */
      1, /* sizeof(buff),*/        /* Maximum size of data packet*/
      &dl);                 /* Length of data packet      */
#endif //0
     return nbytesRead;
 }

volatile unsigned char UARTnTxEmpty[4]={1,1,1,1};
volatile unsigned int DMATCCount = 0;

extern "C" void UART_printf(char port, char *arg_list, ...);
extern "C" int UARTnInit(int nport, int baudrate );
extern "C" int AddUART_TX_to_buffer(int nport, char byte);

extern "C" void UARTSend( unsigned int portNum, unsigned  char *BufferPtr,  unsigned int Length );

/*****************************************************************************/
void UARTSend( unsigned int portNum, unsigned  char *BufferPtr,  unsigned int Length )
{  
    DWORD  numbytes_ok; 
    int l, rc;
	if(portNum >= 4)
       return;		 

   UARTnTxEmpty[portNum] = 0;
printf("todo %s\n",__FUNCTION__ );
#if 0
  rc = WriteFile(Hcom, BufferPtr, Length, &numbytes_ok, NULL); 
    while ( Length != 0 )
    { AddUART_TX_to_buffer(portNum, *BufferPtr);	
	  BufferPtr++;
	  Length--;
	}
   UARTnTxEmpty[portNum] = 1;
#endif //0
  return;
}


void UART_printf(char port, char *arg_list, ...)
{ static char tobuf[512]; //[80];
  va_list arg_buffer;
  va_start(arg_buffer,arg_list);
  vsprintf(tobuf, arg_list, arg_buffer);
  UARTSend(port,(unsigned  char *)&tobuf[0],strlen(&tobuf[0]));
}

int UARTnInit(int nport, int baudrate )
{ // formal
	return 0;
}
