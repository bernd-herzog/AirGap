#include "FirFilter.h"

extern void(*FirFilter_ReportData)(short *data);
extern void FirFilter_OnData(short *data);

void FirFilter_OnData(short *data)
{
	//do something with data
	FirFilter_ReportData(data);
}
