#include "ComplexToSimple.h"
#include "DataTypes.h"

extern void(*ComplexToSimple_ReportData)(FloatPackage);
extern void ComplexToSimple_OnData(ComplexPackage);

void ComplexToSimple_OnData(ComplexPackage packet)
{
	FloatPackage ret;
	//do something with data
	ComplexToSimple_ReportData(ret);
}
