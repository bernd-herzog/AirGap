#include "FloatToShort.h"

extern void(*FloatToShort_ReportData)(ShortPackage);
extern void FloatToShort_OnData(FloatPackage);

void FloatToShort_OnData(FloatPackage packet)
{
	ShortPackage ret;
	//do something with data
	FloatToShort_ReportData(ret);
}
