#include "FloatToShort.h"

extern void(*FloatToShort_ReportData)(FloatPackage);
extern void FloatToShort_OnData(ShortPackage);

void FloatToShort_OnData(FloatPackage packet)
{
	ShortPackage ret;
	//do something with data
	FloatToShort_ReportData(ret);
}
