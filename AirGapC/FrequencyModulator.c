#include "FrequencyModulator.h"
#include "DataTypes.h"

extern void(*FrequencyModulator_ReportData)(ComplexPackage);
extern void FrequencyModulator_OnData(FloatPackage);

void FrequencyModulator_OnData(FloatPackage data)
{
	ComplexPackage ret;
	//do something with data
	FrequencyModulator_ReportData(ret);
}
