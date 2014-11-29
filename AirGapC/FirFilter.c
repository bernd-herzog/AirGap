#include "FirFilter.h"

extern void(*FirFilter_ReportData)(FloatPackage);
extern void FirFilter_OnData(FloatPackage);

void FirFilter_OnData(FloatPackage packet)
{
	//do something with data
	FirFilter_ReportData(packet);
}
