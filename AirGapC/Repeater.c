#include "Repeater.h"
#include "agmath.h"
#include <stdlib.h>

extern void(*Repeater_ReportData)(ComplexPackage);
extern void Repeater_OnData(ComplexPackage);

int numRepeated = ag_SAMPLES_PER_SYMBOL;

void Repeater_OnData(ComplexPackage data)
{
	ComplexPackage ret;
	ret.count = data.count * numRepeated;
	ret.data = (Complex *)calloc(ret.count, sizeof(Complex));

	for (int i = 0; i < data.count; i++)
	{
		for (int j = 0; j < numRepeated; j++)
		{
			ret.data[i*numRepeated + j] = data.data[i];
		}
	}

	Repeater_ReportData(ret);
	free(ret.data);
}
