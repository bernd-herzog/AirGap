#include "Multiply.h"
#include <stdlib.h>
#include <math.h>
#include "agmath.h"

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);
extern void Multiply_SetFrequency(float);

const int sampleRate;
float _frequency;

void Multiply_OnData(ComplexPackage data)
{
	int position = 0;

	ComplexPackage ret;
	ret.count = data.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));

	for (int i = 0; i < data.count; i++)
	{
		position++;
		
		float x = 2 * ag_PI * position / ag_SAMPLERATE * _frequency;
		float x_i = ag_sin(x);
		float x_r = ag_cos(x);

		ret.data[i].i = data.data[i].i * x_r - data.data[i].q * x_i;
		ret.data[i].q = data.data[i].i * x_i + data.data[i].q * x_r;
	}
	
	Multiply_ReportData(ret);
	free(ret.data);
	return;
}

void Multiply_SetFrequency(float frequency)
{
	_frequency = frequency;
}