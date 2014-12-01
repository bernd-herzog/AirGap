#include "FirFilter.h"

extern void(*FirFilter_ReportData)(ComplexPackage);
extern void FirFilter_OnData(ComplexPackage);

extern void FirFilter_InitGaussian();
extern void FirFilter_InitLowPass();


float *taps;
int numTaps;

void FirFilter_OnData(ComplexPackage packet)
{



	
	FirFilter_ReportData(packet);
}

void FirFilter_InitGaussian()
{

}

void FirFilter_InitLowPass()
{

}
