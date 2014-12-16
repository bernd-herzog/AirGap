#include "ClockRecovery.h"
#include "DataTypes.h"
#include <stdlib.h>
#include "agmath.h"

extern void(*ClockRecovery_ReportData)(FloatPackage);
extern void ClockRecovery_OnData(FloatPackage);
void ClockRecovery_Init();

float *_lastValues;
float *_lastAverages;
int _collectedValues = 0;

bool _stateSearching = true;
int _offset = 0;
int _ClockRecovery_position = 0;

#define barrier 0.95f


void ClockRecovery_OnData(FloatPackage packet)
{
	FloatPackage ret;
	ret.data = (float *)calloc(packet.count, sizeof(float));

	int numByte = 0;

	for (int j = 0; j < packet.count; j++)
	{
		float average = 0.f;
		for (int i = 0; i < ag_SAMPLES_PER_SYMBOL - 1; i++)
		{
			_lastValues[i] = _lastValues[i+1];
			average += _lastValues[i];
		}

		_lastValues[ag_SAMPLES_PER_SYMBOL - 1] = packet.data[j];

		average += packet.data[j];
		average /= ag_SAMPLES_PER_SYMBOL;

		for (int i = 0; i < ag_SAMPLES_PER_SYMBOL - 1; i++)
		{
			_lastAverages[i] = _lastAverages[i+1];
		}

		_lastAverages[ag_SAMPLES_PER_SYMBOL - 1] = average;

		if (_collectedValues < ag_SAMPLES_PER_SYMBOL)
			_collectedValues++;
		else
		{
			if (_stateSearching)
			{
				int maxPosition = -1;
				float maxValue = 0;

				for (int i = 0; i < ag_SAMPLES_PER_SYMBOL; i++)
				{
					float absValue = ag_abs(_lastAverages[i]);
					if (absValue > maxValue)
					{
						maxValue = absValue;
						maxPosition = i;
					}
					
					if (maxPosition != -1 && maxPosition > barrier)
					{
						_stateSearching = false;
						_offset = (_ClockRecovery_position + maxPosition) % ag_SAMPLES_PER_SYMBOL;

						ret.data[numByte++] = _lastAverages[i];
					}
				}

			}
			else{
				if (_offset == _ClockRecovery_position)
				{
					ret.data[numByte++] = _lastAverages[ag_SAMPLES_PER_SYMBOL - 1];
				}
			}
		}

		_ClockRecovery_position = (_ClockRecovery_position + 1) % ag_SAMPLES_PER_SYMBOL;
	}


	ret.count = numByte;

	ClockRecovery_ReportData(ret);
	free(ret.data);
}

void ClockRecovery_Init()
{
	_lastValues = (float *)calloc(ag_SAMPLES_PER_SYMBOL, sizeof(float));
	_lastAverages = (float *)calloc(ag_SAMPLES_PER_SYMBOL, sizeof(float));

}