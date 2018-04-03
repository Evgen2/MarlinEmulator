/* mybuffer.hpp */
#ifndef MYBUFFER
#define MYBUFFER

#if 0
//������
class myBuffer
{
 public:
   char *pbuf; // ��������� �� �����
   int Lbuf;  // ����� ������ � ������
   int ibuf;  // ������ ������ ������� ������� 
   int ibuf2; // ������ ������ ����������� ������� 
   int ifree; // ������ ������ ��������� ������� 
//   int Litem; // ����� �������� � int, 0=���������� ����� ��������

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
   int Litem; // ����� �������� � int, 0=���������� ����� ��������
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
   int *pbuf; // ��������� �� �����
   int Lbuf;  // ����� ������ � int
   int ibuf;  // ������ ������ ������� ������� � int
   int ifree; // ������ ������ ��������� ������� � int
   int Litem; // ����� �������� � int, 0=���������� ����� ��������

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
