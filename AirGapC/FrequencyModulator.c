#include "FrequencyModulator.h"
#include "DataTypes.h"
#include <stdlib.h>
#include "agmath.h"

#include <time.h>

extern void(*FrequencyModulator_ReportData)(ComplexPackage);
extern void FrequencyModulator_OnData(ComplexPackage);

float _position = .0f;

void FrequencyModulator_OnData(ComplexPackage data)
{
	clock_t start = clock();

	ComplexPackage ret;
	ret.count = data.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));

	for (int i = 0; i < data.count; i++)
	{
		_position += data.data[i].i * 2 * ag_PI / ag_SAMPLERATE * ag_FREQUENCY_SHIFT;

		while (_position > 8 * ag_PI)
			_position -= 2 * ag_PI;

		while (_position < 0)
			_position += 2 * ag_PI;


		//float x = 2 * ag_PI * position / ag_SAMPLERATE * (data.data[i].i * ag_FREQUENCY_SHIFT);
		ret.data[i].i = ag_cos(_position);
		ret.data[i].q = ag_sin(_position);
	}

	clock_t end = clock();
	float duration = ((float)(end - start)) / CLOCKS_PER_SEC;

	printf("FrequencyMod Calculated %d samples in %f s\n", data.count, duration);

	FrequencyModulator_ReportData(ret);
	free(ret.data);
}
