#include "ShortToFloat.h"

extern void(*ShortToFloat_ReportData)(FloatPackage);
extern void ShortToFloat_OnData(ShortPackage);

void ShortToFloat_OnData(ShortPackage data)
{
	//do something with data
	FloatPackage ret;
	ShortToFloat_ReportData(ret);
}
