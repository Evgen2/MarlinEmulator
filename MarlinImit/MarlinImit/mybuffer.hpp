/* mybuffer.hpp */
#ifndef MYBUFFER
#define MYBUFFER

class myBuffer
{
 public:
   char *pbuf; // указатель на буфер
   int Lbuf;  // длина буфера в байтах
   int ibuf;  // индекс начала зан€той области 
   int ibuf2; // индекс начала прочитанной области  
   int ifree; // индекс начала свободной области 
//   int Litem; // длина элемента в int, 0=переменна€ длина элемента

   myBuffer(void)
   {  pbuf = NULL;
      Lbuf = 0;
      ibuf = ibuf2 = ifree = 0;
//      Litem = 0;
   };
    void Init(char *pb, int Lb);
	  int Add(char byte);
    int Add(char *pb, int Lb);
//    int Add2(int *pb, int Lb);
    int Get(void);
    void StartRead(void);
    int GetUnread(void);
    int Read(char *el);
    void EndRead(void);
    int GetFree(void);
    int GetLbuf(void);
};

class myBuffer2:myBuffer
{
 public:
   int Litem; // длина элемента в int, 0=переменна€ длина элемента
   myBuffer2(void)
   {  Litem = 0;
   };
   void Init(void *pb, int Lb, int _Litem);
	 int Add(void *prt);
   int Get(void *prt);
   int GetLbuf(void);
};

/*************************************************************/

class Waiting
{
 public:
   int stsFrom;
   unsigned int timeout;
   unsigned int timestart;
   unsigned int dt;
   int type[10]; //0 - текст, 1 = +IPD,id,len:|data
   char *ptr[10];
   int len[10];
   int le[10];
   int stsCode[10];
   int stsTimeout;
   int stsNOP;
   int isTimeout;
   int lmin;
   int sts;
   int ldata;
   int nvar;  // число вариантов ожидани€
   myBuffer *bIn;

   Waiting(void)
   { timeout = 1000;
     timestart = 0;
	 bIn = NULL;
	 lmin = 0;
	 sts = 0;
	 ldata = 0;
	 dt = 0;
	 stsTimeout = 0;
	 stsFrom = 0;
		  isTimeout = 0;
   }
   void clear(void)
   {  nvar = 0;
      sts = 0;
	  stsFrom = 0;
		  isTimeout = 0;
   }
   void add(char *_ptr, int _type, int _len, int _stsCode)
	{	if(nvar < 10)
		{  type[nvar] = _type;
           ptr[nvar] =  _ptr;
		   len[nvar] = _len;
		   stsCode[nvar] = _stsCode;
		   nvar++;
		}
   }
    void Start(int _stsNOP, int _stsTimeout)
	{   int i,l;
	    l=0;
	    for(i=0; i<nvar; i++)
		{  if(type[i] == 0)
			{   if(l == 0)
			       l = len[i];
				else if(len[i] < l)
			       l = len[i];
			} else {
				if(l == 0) /* минимум 9байт= +IPD,0,1: */
					l = 9;
				else if(l > 9)
					l = 9;
			}
		    le[i] = 0;
		}
		lmin = l;
		sts = 0;
		ldata = 0;
		dt = 0;
		isTimeout = 0;
		stsNOP = _stsNOP;
		stsTimeout = _stsTimeout;
		bIn->StartRead();
		timestart = LPC_TIM1->TC;
	}
    int Check(void);
};
/*************************************************************/

class WaitData
{
 public:
   unsigned int timeout;
   unsigned int timestart;
   unsigned int dt;
   int len;
   int stsCode;
   int stsTimeout;
   int stsNOP;
   myBuffer *bIn;

   WaitData(void)
   { timeout = 1000;
     timestart = 0;
	 bIn = NULL;
	 len = 0;
	 dt = 0;
	 stsTimeout = 0;
   }

  void Start(int _stsCode, int _stsNOP, int _stsTimeout, int _len)
	{   stsCode = _stsCode;
		len = _len;   
		dt = 0;
		stsNOP = _stsNOP;
		stsTimeout = _stsTimeout;
		bIn->StartRead();
		timestart = LPC_TIM1->TC;
	}
    int Check(void);
};


#endif // MYBUFFER
