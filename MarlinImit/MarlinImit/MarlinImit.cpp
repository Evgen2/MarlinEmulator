// MarlinImit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sim_version.hpp"
#include "SiM_Virtual.hpp"

extern "C" int main_TcpNet(void);
extern "C" void UdpSend(void);
extern "C" int SiMStolInit(void);
extern int SiMreadCfg(void);

char * PrintWinVers(void);
class SiM_VirtualSetup machine;

char  ProgammNameVersion[120]; //="Marlin emulator v.0";

int _tmain(int argc, _TCHAR* argv[])
{  int rc;
rc = SetConsoleCP(1251);
rc = SetConsoleOutputCP(1251);	
   sprintf(ProgammNameVersion, "%s vers %s.%s build %s", APPLICATION_NAME, VERSION, SUBVERSION,  __DATE__);
   printf("%s\n",ProgammNameVersion);
   PrintWinVers();
   SiMreadCfg();
/*
  rc = machine.InitUDP(1);
  if(rc)
	  exit(1);
*/
  rc = machine.InitTCP(1);
  if(rc)
	  exit(1);
   
  rc = machine.Init(0);
  if(rc)
	  exit(1);
   rc = machine.InitConnection();
   if(rc)
	   exit(1);
	
	return 0;
}


int main_TcpNet(void)
{  int rc=0;
#if 0
   rc = machine.Read();
   if(rc > 0)
	   machine.CallBack();
#endif
   return rc;
}

void UdpSend(void)
{   int rc;
#if 0
	   rc = machine.Send();
#endif
}  

extern "C" void TcpSend(void);

void TcpSend(void)
{   int rc;
extern int TcpLsend;
extern unsigned char *TcpMsgOut;

    if (TcpLsend > 0)
	{
       machine.SendTCP(TcpMsgOut, TcpLsend);

      TcpLsend = 0;	
	}
}

/****************************************************/
extern "C" int SiMStolInit(void);

//const char stol_4::BiosDate[12]=__DATE__;     /* дата компиляции биоса */
int SiMStolInit(void)
{
#if 0
	 stol.lpc[0].TargetOut = 0x04;
     stol.lpc[1].TargetOut = 0x04;
     stol.lpc[2].TargetOut = 0x04;
     stol.lpc[3].TargetOut = 0x04;
	 stol.Control = 1;
 	 stol.Init();
//	 stol.isSSP_ADC = 1;
#endif //0
    return 0;
}


unsigned char * udp_get_buf(int Nsend)
{  if(Nsend < sizeof(machine.WriteBuff) )
    {  return (unsigned char *)machine.WriteBuff;
    }
    return NULL;
}

unsigned char * tcp_get_buf(int Nsend)
{  if(Nsend < sizeof(machine.TcpWriteBuff) )
    {  return (unsigned char *)machine.TcpWriteBuff;
    }
    return NULL;
}

 typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL); 
bool getWindowsBit(bool & isWindows64bit)
{
#if _WIN64

    isWindows64bit =  true;
    return true;

#elif _WIN32

    BOOL isWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    LPFN_ISWOW64PROCESS fnIsWow64Process  = (LPFN_ISWOW64PROCESS)
		GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &isWow64))
            return false;

        if(isWow64)
            isWindows64bit =  true;
        else
            isWindows64bit =  false;

        return true;
    }
    else
        return false;

#else

    assert(0);
    return false;

#endif
}

static char EmulatorInfo[128];
char * PrintWinVers(void)
{	OSVERSIONINFO	VersionInfo;
    bool isWindows64bit;
	ZeroMemory( &VersionInfo, sizeof(VersionInfo) );
	VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo );

    GetVersionEx( &VersionInfo );
     getWindowsBit(isWindows64bit);
	 sprintf(EmulatorInfo, "%s run on Windows ver. %i.%i ",ProgammNameVersion, VersionInfo.dwMajorVersion,VersionInfo.dwMinorVersion);
	 if(isWindows64bit) strcat(EmulatorInfo, "64bit\n");
	 else               strcat(EmulatorInfo, "32bit\n");

	return EmulatorInfo;
}