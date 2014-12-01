#include "Multiply.h"
#include <stdlib.h>
#include <math.h>

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);

void Multiply_OnData(ComplexPackage data)
{
	//do something with data
	static long l = 0;

	const plen = 195382;

	while (true){
		ComplexPackage ret;

		ret.count = plen;
		ret.data = (Complex *)malloc(plen * sizeof(Complex));

		for (int i = 0; i < plen; i++)
		{
			float x = (float)l++ * 2.f * 3.1415f;
			float y = sinf(x/44.05f);

			ret.data[i].i = y*0.1f;
			ret.data[i].q = 0.0f;
		}

		Multiply_ReportData(ret);
		free(ret.data);
	}
}
