#include "Multiply.h"
#include <stdlib.h>
#include <math.h>
#include "agmath.h"

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);
extern void Multiply_SetFrequency(float);

float _frequency;

float _Multiply_position = 0;

void Multiply_OnData(ComplexPackage data)
{
	ComplexPackage ret;
	ret.count = data.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));

	for (int i = 0; i < data.count; i++)
	{
		_Multiply_position += 2.f * ag_PI / ag_SAMPLERATE * _frequency;

		if (_Multiply_position > 2.f * ag_PI)
			_Multiply_position -= 2 * ag_PI;

		if (_Multiply_position < 0.f)
			_Multiply_position += 2 * ag_PI;

		float x_i = ag_sin(_Multiply_position);
		float x_r = ag_cos(_Multiply_position);

		ret.data[i].i = data.data[i].i * x_r - data.data[i].q * x_i;
		ret.data[i].q = data.data[i].i * x_i + data.data[i].q * x_r;

		if (ag_abs(ret.data[i].i) > 1.0f || ag_abs(ret.data[i].q) > 1.0)
			printf("übersteuert");
	}

	Multiply_ReportData(ret);
	free(ret.data);
	return;
}

void Multiply_SetFrequency(float frequency)
{
	_frequency = frequency;
}
