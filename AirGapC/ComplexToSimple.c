#include "ComplexToSimple.h"

extern void(*ComplexToSimple_ReportData)(short *data);
extern void ComplexToSimple_OnData(short *data);

void ComplexToSimple_OnData(short *data)
{
	//do something with data
	ComplexToSimple_ReportData(data);
}
