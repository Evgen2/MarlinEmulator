/* SiMconfig.cpp */
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LPC17xx.h"  
#include "Marlin.h"
#include "SiM.hpp"  
#include "Sim_version.hpp"

int Stol_Sim::Read(char *fname)
{  int id,rc, flen;
   FILE *fp;
   char str[256],nameClass[128],name[128],par[128];
   fp = fopen(fname,"r");
//search for .bak if no config or too small
   if(fp == NULL)
   {  sprintf(str,"%s.bak",fname);
      fp = fopen(str,"r");
      if(fp == NULL)
           return 1;
   } else {
      flen = _filelength(_fileno(fp));
      if(flen < 16)
      {  fclose(fp);
         sprintf(str,"%s.bak",fname);
         fp = fopen(str,"r");
         if(fp == NULL)
                 return 1;
      }
   }

   do
   {  rc = ReadStr(fp, str,nameClass,name,par);
      if(!rc)
      { //  if(!_strcmpi(nameClass,"prg"))
            AnalizeRecodrRead(name,par);
      }
   } while(!rc);

   fclose(fp);


   return 0;
}

int Stol_Sim::Write(char *fname, int mode)
{  int id, flen, rc, i;
   FILE *fp;
//   char *prefix="prg.";
   char fnamenew[256], fnameold[256], fnameold2[256];
extern char ProgammNameVersion[];

   fp = fopen(fname,"r");
   if(fp)
   {  flen = _filelength(_fileno(fp));
      fclose(fp);
   } else {
      flen = 0;
   }
   if(flen > 16 && mode) /* переименовываем файл */
   {  static int tren=0;
//      if(abs(clock()-tren) > 60000) // более 1 минуты уже прошло
      {    sprintf(fnameold2,"%s.bak2",fname);
           sprintf(fnameold,"%s.bak",fname);
           rc=remove(fnameold2);
           printf("remove(%s) rc=%i\n",fnameold2,rc);
           rc =rename(fnameold,fnameold2);
           printf("rename(%s,%s) rc=%i\n",fnameold,fnameold2,rc);

           if(rc)
           {   remove(fnameold);
               printf("remove(%s) rc=%i\n",fnameold,rc);
           }
           rename(fname,fnameold);
           printf("rename(%s,%s) rc=%i\n",fname,fnameold,rc);
      }
      tren = clock();
   }

   sprintf(fnamenew,"%s.$",fname);
   fp = fopen(fnamenew,"w");
//   fp = fopen(fname,"w");
   if(fp == NULL)
   {
      printf("Warning: не могу открыть файл %s: errno=%i (%s)\n", fnamenew, errno, strerror(errno));

      return 1;
   }
   fprintf(fp, ";%s\n", ProgammNameVersion);
   fprintf(fp, ";%s %s\n", MSG_MARLIN, SHORT_BUILD_VERSION);
   fprintf(fp, ";Config options:\n");
   fprintf(fp, ";custom machine name: %s\n", CUSTOM_MACHINE_NAME);
   fprintf(fp, ";board name: %s (%i)\n", BOARD_NAME, MOTHERBOARD);
   
    fprintf(fp,";Axis= id used minposition maxposition\n");

	for(i=0; i<4; i++)
	{ fprintf(fp,"Axis= %i %i %f %f\n",i, axis[i].used, axis[i].pos[0], axis[i].pos[1]);
	}
    fprintf(fp,";Endstop: id type(0=min, 1=max) used(0|1) minposition  maxposition\n");
    fprintf(fp,";minposition < position when endstop On < maxposition\n");
	for(i=0; i<4; i++)
	{ fprintf(fp,"Endstop= %i 0  %i %f %f\n",i,  endstopMin[i].used, endstopMin[i].posOn[0], endstopMin[i].posOn[1]);
	  fprintf(fp,"Endstop= %i 1  %i %f %f\n",i,  endstopMax[i].used, endstopMax[i].posOn[0], endstopMax[i].posOn[1]);
	}
	fprintf(fp, ";Heater model. Heater is at layer 0, measured temperature is at last layer\n");
	fprintf(fp, ";Heater model. Bed, extruder1,extruder2\n");
	fprintf(fp, ";used 1/0\n");
	fprintf(fp, "h_used=%i %i %i\n", r1.used, r2.used, r3.used);
	fprintf(fp, ";heaters power\n");
	fprintf(fp, "h_W=%f %f %f\n", r1.W, r2.W, r3.W);
	fprintf(fp, ";number of model layers (1-100), more layers = more heat inertia\n");
	fprintf(fp, "h_n=%i %i %i\n", r1.n, r2.n, r3.n);
	fprintf(fp, ";k1 thermal conductivity coefficient (beetwen layers)\n");
	fprintf(fp, "h_k1=%f %f %f\n", r1.k1, r2.k1, r3.k1);
	fprintf(fp, ";k2 thermal dissipation coefficient (each layer dissipation)\n");
	fprintf(fp, "h_k2=%f %f %f\n", r1.k2, r2.k2, r3.k2);
	fprintf(fp, ";kE thermal dissipation coefficient for first and last layes\n");
	fprintf(fp, "h_kE=%f %f %f\n", r1.kE, r2.kE, r3.kE);
	fprintf(fp, ";t0 start=external temperature \n");
	fprintf(fp, "h_t0=%f %f %f\n", r1.t0, r2.t0, r3.t0);
	fprintf(fp, ";tmax maximu temperature \n");
	fprintf(fp, "h_tmax=%f %f %f\n", r1.tmax, r2.tmax, r3.tmax);

   fclose(fp);
   remove(fname);
   rename(fnamenew,fname);
  return 0;
}

int Stol_Sim::AnalizeRecodrRead(char *name, char *par)
{  int i,is=0,npar=0, rc;

static char *lprgsNames[]=
{ "Axis", "Endstop", 
  "h_used","h_W","h_n","h_k1","h_k2","h_kE","h_t0","h_tmax",
  NULL
};

  for(i=0;lprgsNames[i];i++)
  {
      if(!strcmp(name,lprgsNames[i]))
      {  is = 1;
         npar = i;
         break;
      }
  }
  if(!is) return 1;
  switch(npar)
  {  case 0:  /* Axis */
      { int id, used;
	    double p0, p1;
//	{ fprintf(fp,"Axis %i %i %f %f\n",i, axis[i].used, axis[i].pos[0], axis[i].pos[1]);
        rc = sscanf(par,"%i %i %lf %lf",&id, &used, &p0, &p1);
		if(rc ==4 && id>=0 && id <= 3)
		{ axis[id].used = used;
		  axis[id].pos[0] = p0;
		  axis[id].pos[1] = p1;
		}
      }
       break;
     case 1:  /* Endstop */
      { int id, typ, used;
	    double p0, p1;
//	fprintf(fp,"Endstop %i 0  %i %f %f\n",i, endstopMin[i].isMax, endstopMin[i].used, endstopMin[i].posOn[0], endstopMin[i].posOn[1]);
        rc = sscanf(par,"%i %i %i %lf %lf",&id, &typ, &used, &p0, &p1);
		if(rc ==5 && id>=0 && id <= 3)
		{   if(typ == 0)
		    {	endstopMin[id].used = used;
				endstopMin[id].posOn[0] = p0;
				endstopMin[id].posOn[1] = p1;
				endstopMax[id].isMax = 0;
			} else {
				endstopMax[id].used = used;
				endstopMax[id].posOn[0] = p0;
				endstopMax[id].posOn[1] = p1;
				endstopMax[id].isMax = 1;
			}
		}
      }
       break;
	 case 2: /* *h_used */
	 {  int used1, used2, used3;
	 rc = sscanf(par, "%i %i %i", &used1, &used2, &used3);
	 if (rc >= 1) r1.used = used1;
	 if (rc >= 2) r2.used = used2;
	 if (rc >= 3) r3.used = used3;
	 }
	 break;
	 case 3: /* h_W */
	 {  double w1, w2, w3;
	 rc = sscanf(par, "%lf %lf %lf", &w1, &w2, &w3);
	 if (rc >= 1) r1.W = w1;
	 if (rc >= 2) r2.W = w2;
	 if (rc >= 3) r3.W = w3;
	 }
	 break;
	 case 4: /* *h_n */
	 {  int used1, used2, used3;
	 rc = sscanf(par, "%i %i %i", &used1, &used2, &used3);
	 if (rc >= 1) r1.n = used1;
	 if (rc >= 2) r2.n = used2;
	 if (rc >= 3) r3.n = used3;
	 }
	 break;
	 case 5: /* h_k1 */
	 {  double w1, w2, w3;
	 rc = sscanf(par, "%lf %lf %lf", &w1, &w2, &w3);
	 if (rc >= 1) r1.k1 = w1;
	 if (rc >= 2) r2.k1 = w2;
	 if (rc >= 3) r3.k1 = w3;
	 }
	 break;
	 case 6: /* h_k2 */
	 {  double w1, w2, w3;
	 rc = sscanf(par, "%lf %lf %lf", &w1, &w2, &w3);
	 if (rc >= 1) r1.k2 = w1;
	 if (rc >= 2) r2.k2 = w2;
	 if (rc >= 3) r3.k2 = w3;
	 }
	 break;
	 case 7: /* h_kE */
	 {  double w1, w2, w3;
	 rc = sscanf(par, "%lf %lf %lf", &w1, &w2, &w3);
	 if (rc >= 1) r1.kE = w1;
	 if (rc >= 2) r2.kE = w2;
	 if (rc >= 3) r3.kE = w3;
	 }
	 break;
	 case 8: /* h_t0 */
	 {  double w1, w2, w3;
	 rc = sscanf(par, "%lf %lf %lf", &w1, &w2, &w3);
	 if (rc >= 1) r1.t0 = w1;
	 if (rc >= 2) r2.t0 = w2;
	 if (rc >= 3) r3.t0 = w3;
	 }
		 break;
	 case 9: /* h_tmax */
	 {  double w1, w2, w3;
	 rc = sscanf(par, "%lf %lf %lf", &w1, &w2, &w3);
	 if (rc >= 1) r1.tmax = w1;
	 if (rc >= 2) r2.tmax = w2;
	 if (rc >= 3) r3.tmax = w3;
	 }
		 break;
	 
  }

  return 0;
}

int Stol_Sim::ReadStr(FILE *fp, char *str, char *nameClass, char *name, char *par )
{  int i,iscomment,l;
   char *pstr;
/* читаем строку */
M: pstr= fgets(str,128,fp);
   if(pstr == NULL) return 1; //EOF
/* игнорируем строки нулевой длины */
   l = strlen(str);
   if(str[l-1] == '\n')
   {  str[--l] = 0;
   }
   if(l == 0) goto M;
/* игнорируем комментарии */
   iscomment = 0;
   for(i=0;i<l;i++)
   { if(str[i] > 32)
     {   if(str[i] == ';') iscomment = 1;
         break;
     }
   }
   if(iscomment) goto M;
   pstr = strstr(str,"=");
   if(pstr == NULL)
        goto M; // игнорируем строки без "="
   *pstr = 0;
   strcpy(par,pstr+1);      // читаем параметры
   pstr = strstr(str,".");  // ищем точку для определения имени класса
   if(pstr == NULL)
   {
//было:      goto M; // игнорируем строки без "."
      nameClass[0] = 0;
      sscanf(str,"%s",name);
   } else {
      *pstr = 0;
      sscanf(str,"%s",nameClass);
      sscanf(pstr+1,"%s",name); // читаем имя параметра
   }
   return 0;
}

