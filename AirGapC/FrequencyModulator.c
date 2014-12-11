#include "FrequencyModulator.h"
#include "DataTypes.h"
#include <stdlib.h>
#include "agmath.h"

extern void(*FrequencyModulator_ReportData)(ComplexPackage);
extern void FrequencyModulator_OnData(ComplexPackage);

void FrequencyModulator_OnData(ComplexPackage data)
{
	ComplexPackage ret;
	ret.count = data.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));
	int position = 0;

	for (int i = 0; i < data.count; i++)
	{
		position++;

		float x = 2 * ag_PI * position / ag_SAMPLERATE * (data.data[i].i * 1000);
		ret.data[i].q = ag_sin(x);
		ret.data[i].i = ag_cos(x);
	}

	FrequencyModulator_ReportData(ret);
	free(ret.data);
}
