/* SimTempModel.hpp */
#ifndef SIM_TEMP_MODEL
#define SIM_TEMP_MODEL

#define MAX_NRD 100
class Rod
{ 
public:
	double St[MAX_NRD];
    int n;
	int used;
	double k1;
	double k2;
	double kE;
	double S;
	double W;
	double t0,tmax;
	Rod(void)
	{  int i;
	   for(i=0;i<MAX_NRD;i++) St[i] = 0.;
	   used = 0;
	   n = 2;
	   k1 = 0.1; 
	   k2 = 0.01;
	   kE = 0.01;
	   S = 0.;
	   W = 1.;
	   t0 = 10;
	   tmax = 200.;
	}
	void Set(double t_0)
	{  int i;
	   for(i=0; i<n; i++) St[i] = t_0;
	}
	double Step(double dt, double dQ);
};

#endif