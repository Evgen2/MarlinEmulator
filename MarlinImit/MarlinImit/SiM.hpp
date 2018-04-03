/* SiM.hpp */

#include "SimTempModel.hpp"
#define NOP 0
#define CMDTEST1 1
#define SETMODELPAR 2
#define CMDTEST4 3
#define CMD_CHANGE_MODE 4
#define GETPAR   5  //запрос 0 байт ответ 15
#define SETPAR   6  //запрос 15 байт ответ 1 (rc)
#define SETTRANS 7  //установка режима трансляции аварийных концевиков
#define GETDATA  8  //запрос X и концевиков
#define SETU     9  //установка выходного напряжения
#define SETPOS   10 //установка положения
#define SETKON   11 //установка выходных cигналов
#define SETMDATA 12 //установка параметров модели (для X0-X1)
#define LASTCMD   12

/* из параметры слейва struct LPC_Slave */
class StolSimController
{
 public:
	 int isAct;
  int KonOut;             /* выходные сигналы на P0 */ 
  int Xpos;               /* положение */
  int errXpos;            /* ошибок с датчика положения */
  int Kon0;               /* входные концевики с P0 */
  int Kon1;               /* входные концевики c P1 */ 
  int KonAK;              /* входные аварийные концевики с учётом InverseAK */
  int UoutSet;            /* установить выходное напряжение */
  int Uout;               /* реальное выходное напряжение после сдвигов и ограничений */
  int XposTTLtag;  
	 char OutBuff[1000]; // Выходной буфер
	 int nOut;           // байт в выходном буфере
	 StolSimController(void)
	 {  isAct = 0;
	    Xpos = errXpos = 0;
		Kon0 = Kon1 = 0;
		KonAK = 0;
		Uout = 0;
		UoutSet = 0;
		KonOut = 0;
		XposTTLtag = 0;
	 }
};

class SimEndStop
{   public:
		int used;
		int IsOn;
		int indCoord; // coordinate 0,1,2,3,4
		int isMax;    // 0 - min, 1 - max
		double posOn[2];  // posOn[0] < On position  < posOn[1]
		SimEndStop(void)
		{  IsOn = 0;
		   used = 0;
		   indCoord = 0;
		   isMax = 1;
		   posOn[0] = -1;
		   posOn[1] =  1;
		}
};

class SimAxis
{   public:
		int used;
		int indCoord; // coordinate 0,1,2,3,4
		double pos[2];  // posOn[0] < On position  < posOn[1]
		SimAxis(void)
		{  
		   used = 0;
		   indCoord = 0;
		   pos[0] = -0.5;
		   pos[1] =  100.;
		}
};

class Stol_Sim
{
 public:
  double x[4];
  double x0[4]; // начальное положение
  int  Pos[4];
  
  SimEndStop endstopMin[4];
  SimEndStop endstopMax[4];
  SimAxis axis[4];


  double Mashtab[4];
  double TimerKoeff; // timer coeff 
//  unsigned int SysTickFreq;   //System timer freq
  unsigned int SysTickPeriod;   //System timer period in TC0 ticks
  unsigned int SysTickClockT;   //System timer clock time in TC0 ticks
 
  int inZatvor;  // Out1  «Открыть затвор»
  int inWatch;   // Out4 «Следить»
  int outSE;     // In6  «Разрешение затвора»
  int outZR;     // In7  «Готовность слежения»
  double Thead;  // температура головки
  int tclock;    // время в клоках со старта программы
  unsigned int IncrTimer_t0;
  class Rod r1, r2, r3;

//  StolSimController slave[4];

  Stol_Sim(void)
  {  x[0] = x[1] = x[2] = x[3] = 0.;
     x0[0] = x0[1] = x0[2] = x0[3] = 100.;
     Pos[0] = Pos[1] = Pos[2] =Pos[3] = 0;

	 Mashtab[0] = 0.005;
     Mashtab[1] = 0.005;
     Mashtab[2] = 0.0012;
     Mashtab[3] = 0.005;

	 TimerKoeff = 25;
//	 SysTickFreq = 1000;
	 SysTickClockT = 0;
	 SysTickPeriod = 25000;
    inZatvor = inWatch = 0;
    outSE = outZR = 0;
	Thead = 0.;
	tclock = 0;
	IncrTimer_t0 = 0;
  }
  int SetEndstops(void);
  int IncrTimer(void);
  int IncrTimer(int n);
  int IncrTimer2(void);
  int IncrTimer3(unsigned int  ad_t);
  int TimerCOn(void);
  int SetTimerIncr(int raz, int mc);
  int setActiveController(int ncontroller, int OnOff);
  int SetU(int nslave, short int U);
  int SetSlaveKonOut(int nprivod, int KonOut);
  int SetSlavePos(int nslave, int pos, int mode);
  int GetPar(int nslave);
  int WatchWork(void);
  int Temperature(void);
  int TempImitStep(int heatOn, double dt, double &t1, double &t2);
  int TempImitStep1(int heatOn, int ind, double dt, double &tmp);
  int TempImitStart(void);

  int WriteMessageFromSlave(int nslave, char *buf, int l);

  int Read(char *fname);
  int ReadStr(FILE *fp, char *str, char *nameClass, char *name, char *par );
  int AnalizeRecodrRead(char *name, char *par);

  int Write(char *fname, int mode);
};

