#include "FirFilter.h"

extern void(*FirFilter_ReportData)(ComplexPackage);
extern void FirFilter_OnData(ComplexPackage);

void FirFilter_OnData(ComplexPackage packet)
{
	//do something with data
	FirFilter_ReportData(packet);
}
