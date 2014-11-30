#include "Multiply.h"
#include <stdlib.h>
#include <math.h>

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);

void Multiply_OnData(ComplexPackage data)
{
	//do something with data
	static long l = 0;

	while (true){
		ComplexPackage ret;

		ret.count = 921;
		ret.data = (Complex *)malloc(921 * sizeof(Complex));

		for (int i = 0; i < 921; i++)
		{
			float x = (float)l++ * 2.f * 3.1415f;
			float y = sinf(x/44.05f);

			ret.data[i].i = y;
			ret.data[i].q = 0.0f;
		}

		Multiply_ReportData(ret);
		free(ret.data);
	}
}
