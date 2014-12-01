#include "FrequencyModulator.h"
#include "DataTypes.h"
#include <stdlib.h>

extern void(*FrequencyModulator_ReportData)(ComplexPackage);
extern void FrequencyModulator_OnData(ComplexPackage);

void FrequencyModulator_OnData(ComplexPackage data)
{
	ComplexPackage ret;
	ret.count = data.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));
	float x = 0.0;

	for (int i = 0; i < data.count; i++)
	{
		x += data.data[i].i;

		//TODO: sin & cos von x ausrechnen und in ret.data[i] schreiben
	}

	FrequencyModulator_ReportData(ret);
	free(ret.data);
}
