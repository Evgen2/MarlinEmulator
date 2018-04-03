/* SimTempModel.cpp */
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SiM.hpp"  


double Rod::Step(double dt, double dQ)
{   int j,i;
    double q1,q2, dt1, dt2;
    double St_tmp[MAX_NRD];
	
	dt1 = dt*0.5;

   for(j=0; j<2; j++)
   {
	   St[0] += dQ*dt1;
	   St[0] -= (St[0] - t0) * kE * dt1;
	 for(i=0; i<n; i++)
	 {	q1 = (St[i] - t0) * k2; // потери
		q2 = 0.;
		if(i > 0)
		{  q2 += (St[i-1] - St[i]) *k1; 
		}
		if(i < n-1)
		{   q2 -= (St[i] - St[i+1]) *k1;
		}
		St_tmp[i] = St[i] + (q2 - q1)*dt1;
		if(St_tmp[i] < t0)
			St_tmp[i] = t0;
		else if(St_tmp[i] > tmax)
			St_tmp[i] = tmax;
	 }
//	 St_tmp[0] -= (St[0] - t0) * kE * dt1;
	 St_tmp[n - 1] -= (St[n - 1] - t0) * kE * dt1;

//     St_tmp[0] += dQ*dt1;
 	 S = 0.;
	 for(i=0; i<n; i++)
	 {   St[i] = St_tmp[i];
	     S += St[i];
	 }
   }
	return St[n-1];
}

/*
int TempImitStart(void)
{  int i;
   r1.n = 100;
   r2.n = 90;
   r3.n = 85;
   r1.k1 = 8.;
   r1.k2 = 0.0011;
   r2.k1 = 4;
   r2.k2 = 0.0001;
   r3.k1 = 4.;
   r3.k2 = 0.0012;
   r1.W = 40.;
   r2.W = 0.1;
   r3.W = 20.2;
   r1.t0 = r2.t0 = r3.t0 = 20.;
   r1.Set(20.);
   r2.Set(20.);
   r3.Set(20.);
   return 0;
}

*/
int Stol_Sim::TempImitStart(void)
{
	if (r1.used)
		r1.Set(r1.t0);
	if (r2.used)
		r2.Set(r2.t0);
	if (r3.used)
		r3.Set(r3.t0);
	return 0;
}
//int TempImitStep(int h1,int h2,int h3, double dt)
int Stol_Sim::TempImitStep(int heatOn, double dt, double &t1, double &t2)
{
   double t0[3], tend[3], q1,q2;
extern float Temp[2];

tend[0] = r1.Step(dt, r1.W*heatOn);
tend[1] = r2.Step(dt, 0.);
//   tend[2] = r3.Step(dt, r3.W*h3);
/* Взаимное влияние */
   q1 = (r1.St[r1.n/2] - r2.St[r2.n/2]) *r1.k1*dt; 
   q2 = (r2.St[r2.n/2] - r3.St[r3.n/2]) *r2.k1*dt; 
   r1.St[r1.n/2] -= q1;
   r2.St[r2.n/2] += q1;
//   r2.St[r2.n/2] -= q2;
//   r3.St[r3.n/2] += q2;

   t1 = r1.St[r1.n - 1];
   t2 = r2.St[r2.n-1];
   return 0;
}

int Stol_Sim::TempImitStep1(int heatOn, int ind, double dt, double &tmp)
{
	double tend[3];
	switch (ind)
	{	case 0:
			tend[0] = r1.Step(dt, r1.W * heatOn);
			tmp = r1.St[r1.n - 1];
			break;
		case 1:
			tend[1] = r2.Step(dt, r2.W * heatOn);
			tmp = r2.St[r2.n - 1];
			break;
		case 2:
			tend[2] = r3.Step(dt, r3.W * heatOn);
			tmp = r3.St[r3.n - 1];
			break;
	}
	return 0;
}

/*
int TempTest(void)
{  int i;
   double t0[3], tend[3], t, dt;
   r1.n = 100;
   r2.n = 90;
   r3.n = 85;
   r1.k1 = 5.;
   r1.k2 = 0.001;
   r2.k1 = 5.;
   r2.k2 = 0.00099;
   r3.k1 = 5.;
   r3.k2 = 0.0011;

   t = 0;
   dt = 0.1;
   for(i=0;i< 100000;i++)
   {  tend[0] = r1.Step(dt, 10.);
      t0[0] = r1.St[0];
	  tend[1] = r2.Step(dt, 10.);
      t0[1] = r2.St[0];
	  tend[2] = r3.Step(dt, 10.);
      t0[2] = r3.St[0];
	  t += dt;
	  if(i%400 == 0)
	   {    printf("%f %f %f ", t, t0[0], tend[0]);
	        printf("  %f %f ",  t0[1], tend[1]);
	        printf("  %f %f\n", t0[2], tend[2]);
	  }
   }
   return 0; 
}
*/


