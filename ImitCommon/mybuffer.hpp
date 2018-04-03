/* mybuffer.hpp */
#ifndef MYBUFFER
#define MYBUFFER

#if 0
//старое
class myBuffer
{
 public:
   char *pbuf; // указатель на буфер
   int Lbuf;  // длина буфера в байтах
   int ibuf;  // индекс начала занятой области 
   int ibuf2; // индекс начала прочитанной области 
   int ifree; // индекс начала свободной области 
//   int Litem; // длина элемента в int, 0=переменная длина элемента

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
   int Litem; // длина элемента в int, 0=переменная длина элемента
   myBuffer2(void)
   {  Litem = 0;
   };
   void Init(void *pb, int Lb, int _Litem);
	 int Add(void *prt);
   int Get(void *prt);
   int GetLbuf(void);
};

#endif //0

class myBuffer
{
 public:
   int *pbuf; // указатель на буфер
   int Lbuf;  // длина буфера в int
   int ibuf;  // индекс начала занятой области в int
   int ifree; // индекс начала свободной области в int
   int Litem; // длина элемента в int, 0=переменная длина элемента

   myBuffer(void)
   {  pbuf = NULL;
      Lbuf = 0;
      ibuf = ifree = 0;
      Litem = 0;
   };
    void Init(int *pb, int Lb);
    int Add(int *pb, int Lb);
    int Add2(int *pb, int Lb);
    int Get(int *pb);
    int GetFree(void);
    int GetFree2(void);
    int GetLbuf(void);
    int GetLbuf2(void);
};

#endif // MYBUFFER
