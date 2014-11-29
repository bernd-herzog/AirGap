#include "SimpleToComplex.h"
#include "DataTypes.h"

extern void(*SimpleToComplex_ReportData)(ComplexPackage);
extern void SimpleToComplex_OnData(FloatPackage);

void SimpleToComplex_OnData(FloatPackage packet)
{
	//do something with data
	ComplexPackage ret;
	SimpleToComplex_ReportData(ret);
}
