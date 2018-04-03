/* mybuffer.cpp */
#include "LPC17xx.h"		// Keil: Register definition file for LPC17xx
#include <stdlib.h>
#include <string.h>

#include "mybuffer.hpp"

/* ������ ��� myBuffer lb ���� �� ������ pb */
void myBuffer::Init(char *pb, int Lb)
{   pbuf = pb;
    Lbuf = Lb;
    ibuf = ifree = 0;	 
}

/* ������ ��� myBuffer lb ���� �� ������ pb, ���������� ����� ��������  _Litem */
void myBuffer2::Init(void *pb, int Lb, int _Litem)
{   pbuf = (char *)pb;
    Litem	= _Litem;
    Lbuf = Lb/Litem*Litem; // Lbuf ������������� �� ����� ��������
    ibuf = ifree = 0;
}

/* �������� � myBuffer Lb ���� � ������ pb */
int myBuffer::Add(char *pb, int Lb)
{ 
	if(GetFree() <= Lb)
		  return 0;

	if((ifree + Lb) < Lbuf)
	{	memcpy((void *)&pbuf[ifree],(void *)pb ,Lb);
		ifree += Lb;
	} else {
		int k0, k1;
		k0 = ifree + Lb -	Lbuf;
		k1 = Lb - k0;
		memcpy((void *)&pbuf[ifree],(void *)pb, k1 );
		if(k0)
		   memcpy((void *)&pbuf[0], (void *)&pb[k1], k0);
		ifree = k0;
		
	}
	return 1;
}

/* �������� � myBuffer ���� ���� */
int myBuffer::Add(char byte)
{ 
	if(GetFree() <= 1)
		  return 0;
  pbuf[ifree] = byte;
	if((ifree + 1) < Lbuf)
	{	ifree++;
	} else {
		ifree = 0;
	}
	return 1;
}

/* �������� � myBuffer2 ���� ������� */
int myBuffer2::Add(void *ptr)
{  int is_wrap = 0, l;
	 l = GetFree();
	if(l < Litem*sizeof(int))
		  return 0;
	if(l == Litem*sizeof(int)) 
		 is_wrap = 1;
	
	memcpy((void *)&pbuf[ifree], ptr, Litem*sizeof(int));
	if((ifree + Litem*sizeof(int)) < Lbuf)
	{	ifree +=Litem*sizeof(int);
	} else {
		ifree = 0;
	}
	if(is_wrap)
	{ if((ibuf + Litem*sizeof(int)) < Lbuf)
		{	ibuf +=Litem*sizeof(int);
		} else {
			ibuf = 0;
		}
	}
	return 1;
}

/* ������� ������� ������ � ���������� ����� */
int myBuffer::Get(void)
{   int rc; 
	if(GetLbuf() <= 0)
		  return -1;
	 rc = pbuf[ibuf];
   ibuf++;	
    if(ibuf == Lbuf)
			     ibuf = 0;
	return rc;
}

/* ������� ������� ������ � ���������� ����� */
int myBuffer2::Get(void *ptr)
{   
	if(GetLbuf() <= 0)
		  return -1;
   memcpy(ptr,(void *)&pbuf[ibuf], Litem*sizeof(int));
   ibuf += Litem*sizeof(int);	
    if(ibuf == Lbuf)
			     ibuf = 0;
	return 0;
}

/**************************************************/
/* ������� ������� ������ � � �� ���������� ����� */
/* ������ ������ */
void myBuffer::StartRead(void)
{    ibuf2 = ibuf;
}
/* ��������� ������ � ���������� ����� � ������ */
void myBuffer::EndRead(void)
{
	 ibuf = ibuf2;
}

int myBuffer::GetUnread(void)
{   int l, l2;
/*******************/
     l = ifree - ibuf;
	 if(l < 0) l += Lbuf;	// l = ����� ������
	 if(l <= 0)
		 return 0; // ����� ������
//  ibuf...................ifree
//  ....|...ibuf2.........|.....
     l2 = ibuf2 - ibuf;
	 if(l2 < 0) l2 += Lbuf;	// l2 = ����� ����������� ����� ������
	 if(l2 >= l)
		 return 0; // ����� ��������
	 return l - l2;
}

/* ���������� ������� ������� ������ � �� ���������� ����� */
int myBuffer::Read(char *el)
{   int  l, l2; 
/*******************/
     l = ifree - ibuf;
	 if(l < 0) l += Lbuf;	// l = ����� ������
	 if(l <= 0)
		 return -1; // ����� ������
//  ibuf...................ifree
//  ....|...ibuf2.........|.....
     l2 = ibuf2 - ibuf;
	 if(l2 < 0) l2 += Lbuf;	// l2 = ����� ����������� ����� ������
	 if(l2 >= l)
		 return -2; // ����� ��������

/******************/
	*el = pbuf[ibuf2];
    ibuf2++;	
    if(ibuf2 == Lbuf)
			     ibuf2 = 0;
	return 0;
}


/* ������� �������� � ������ */
int myBuffer::GetFree(void)
{ 
	return Lbuf - GetLbuf();
}

/* ����� �������� ������ � char */
int myBuffer::GetLbuf(void)
{  int l;
     l = ifree - ibuf;
	 if(l < 0) l += Lbuf;	
	return l;
}
/* ����� �������� ������ � ��������� */
int myBuffer2::GetLbuf(void)
{  int l;
     l = ifree - ibuf;
	 if(l < 0) l += Lbuf;	
	return l/(Litem*sizeof(int));
}

#if 0 //poka	

/* ������� �������� � ��������� � ������ */
int myBuffer::GetFree2(void)
{  int l;
     l = ifree - ibuf;
	 if(l < 0) l += Lbuf;
  l = Lbuf - l;
	return l/Litem;
}

#endif //0
