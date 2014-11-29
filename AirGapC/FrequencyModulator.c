#include "FrequencyModulator.h"
#include "DataTypes.h"

extern void(*FrequencyModulator_ReportData)(ComplexPackage);
extern void FrequencyModulator_OnData(ComplexPackage);

void FrequencyModulator_OnData(ComplexPackage data)
{
	ComplexPackage ret;
	ret.count = 0;
	ret.data = 0;
	//do something with data
	FrequencyModulator_ReportData(ret);
}
