// MarlinPinExist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int Generate(char *fname)
{  FILE *fp, *fp1;
   int i, n, l;
   char str[320];
   char *pstr, *pstr1;
   fp = fopen(fname,"r");
   if(fp == NULL)
   {   printf("no file %s\n", fname);
       return 1;
   }
   fp1 = fopen("pinexist.h","w");
   fprintf(fp1,"/* pinexist.h genetated by MarlinPinExist.exe */\n");

   for(i=0; ;i++)
   {   pstr= fgets(str, 320,fp); /* читаем строку */
	   if(pstr == NULL) break; //EOF
/* игнорируем строки нулевой длины */
		l = strlen(str);
		if(str[l-1] == '\n')
		{  str[--l] = 0;
		}
		if(l == 0) continue;
		pstr = strstr(str,"#if");
		if(pstr)
		{  fprintf(fp1,"%s\n", str);
		   continue;
		}
		pstr = strstr(str,"#endif");
		if(pstr)
		{  fprintf(fp1,"%s\n", str);
		   continue;
		}
		pstr = strstr(str,"#else");
		if(pstr)
		{  fprintf(fp1,"%s\n", str);
		   continue;
		}
		pstr = strstr(str,"#elif");
		if(pstr)
		{  fprintf(fp1,"%s\n", str);
		   continue;
		}

		pstr = strstr(str,"//");
		if(pstr)
			*pstr = 0;
		l = strlen(str);
		if(l == 0) continue;

//#define TMP03_1_PIN
		pstr = strstr(str,"#define");
		if(pstr == NULL)
			continue;
		pstr += 7;
		while(*pstr <= 32 && *pstr) pstr++; //пробелы и табуляции

		pstr1 = strstr(str,"_PIN");
		if(pstr1 == NULL)
			continue;
		 pstr1 += 4;
		 if(*pstr1 >32)
			 continue;
		*pstr1 = 0;
		printf("%s\n", pstr);		
/*
#if defined(TEMP_0_PIN) && TEMP_0_PIN >= 0
	#define TEMP_0_PIN_EXIST 1
#else
	#define TEMP_0_PIN_EXIST 0
#endif
*/
		 fprintf(fp1,"#if defined(%s) && %s >= 0\n", pstr, pstr);
		 fprintf(fp1,"\t #define %s_EXIST 1\n", pstr);
		 fprintf(fp1,"#else\n");
		 fprintf(fp1,"\t #define %s_EXIST 0\n", pstr);
		 fprintf(fp1,"#endif\n\n");

   }
   fclose(fp);
   fclose(fp1);
   return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Create pinexist.h from pinsxx.h\n");
	printf("usage: MarlinPinExist pinsxx.h \n");
	if(argc != 2)
		return -1;
	Generate(argv[1]);
	return 0;
}

