#include "Repeater.h"
#include <stdlib.h>

extern void(*Repeater_ReportData)(ComplexPackage);
extern void Repeater_OnData(ComplexPackage);

int numRepeated = 100;

void Repeater_OnData(ComplexPackage data)
{
	ComplexPackage ret;
	ret.data = (Complex *)malloc(numRepeated * data.count * sizeof(Complex));

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
