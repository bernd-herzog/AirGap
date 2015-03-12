#include "ClockRecovery.h"
#include "DataTypes.h"
#include <stdlib.h>
#include "agmath.h"
#include <string.h>
#include "Depacketizer.h"

extern void(*ClockRecovery_ReportData)(FloatPackage);
extern void ClockRecovery_OnData(FloatPackage);
void ClockRecovery_Init();

float *_lastValues;
float *_lastAverages;
int _collectedValues = 0;

int _ClockRecovery_offset = 0;
int _ClockRecovery_position = 0;

#define barrier 0.6f
int lastMaxPosition = 0;

float _ClockRecovery_lastBuffer[ag_SAMPLES_PER_SYMBOL];
int _ClockRecovery_lastBufferPosition = ag_SAMPLES_PER_SYMBOL - 1;
int _ClockRecovery_samplesSinceLastBit = 0;

int _ClockRecovery_lastPositions[ag_SAMPLES_PER_SYMBOL];
int _ClockRecovery_lastPositionsPosition = ag_SAMPLES_PER_SYMBOL - 1;
int _ClockRecovery_averagePosition = 0;

void ClockRecovery_OnData(FloatPackage packet)
{
	START_TIMER;

	int numBits = 0;
	FloatPackage ret;
	ret.data = (float *)calloc(packet.count, sizeof(float));

	for (int i = 0; i < packet.count; i++)
	{
		_ClockRecovery_lastBufferPosition = (_ClockRecovery_lastBufferPosition + 1) % ag_SAMPLES_PER_SYMBOL;
		_ClockRecovery_lastBuffer[_ClockRecovery_lastBufferPosition] = packet.data[i];

		int lowerPos = (_ClockRecovery_lastBufferPosition + ag_SAMPLES_PER_SYMBOL / 2) % ag_SAMPLES_PER_SYMBOL;
		float lowerValue = _ClockRecovery_lastBuffer[lowerPos];

		int upperPos = (lowerPos + 1) % ag_SAMPLES_PER_SYMBOL;
		float upperValue = _ClockRecovery_lastBuffer[upperPos];

		if (lowerValue * upperValue < 0.f)
		{
			//possible bit detected
			//verify 
			float lowerAverage = 0.f;
			float upperAverage = 0.f;

			for (int j = 0; j < ag_SAMPLES_PER_SYMBOL / 2; j++)
			{
				lowerPos = (_ClockRecovery_lastBufferPosition + 1 + j) % ag_SAMPLES_PER_SYMBOL;
				lowerAverage += _ClockRecovery_lastBuffer[lowerPos];
				upperPos = (lowerPos + ag_SAMPLES_PER_SYMBOL / 2) % ag_SAMPLES_PER_SYMBOL;
				upperAverage += _ClockRecovery_lastBuffer[upperPos];
			}

			lowerAverage /= ag_SAMPLES_PER_SYMBOL / 2;
			upperAverage /= ag_SAMPLES_PER_SYMBOL / 2;

			//printf("values: %d\t%d\t%d\t%f\t%f\n", _ClockRecovery_averagePosition, _ClockRecovery_lastBufferPosition, _ClockRecovery_samplesSinceLastBit, lowerAverage, upperAverage);

			if (lowerAverage * upperAverage < 0.f
				&& ag_abs(lowerAverage) > 0.3f
				&& ag_abs(upperAverage) > 0.3f)
			{
				_ClockRecovery_samplesSinceLastBit = 0;

				_ClockRecovery_lastPositionsPosition = (_ClockRecovery_lastPositionsPosition + 1) % ag_SAMPLES_PER_SYMBOL;
				_ClockRecovery_lastPositions[_ClockRecovery_lastPositionsPosition] = _ClockRecovery_lastBufferPosition;

				const float avgValues[7] = { 0.00443f, 0.05399f, 0.24197f, 0.39894f, 0.24197f, 0.05399f, 0.00443f };
				float avgBuffer[ag_SAMPLES_PER_SYMBOL];
				memset(avgBuffer, 0, sizeof(avgBuffer));

				for (int j = 0; j < ag_SAMPLES_PER_SYMBOL; j++)
				{
					int pos = _ClockRecovery_lastPositions[j];

					for (int j = 0; j < 7; j++)
					{
						int nPos = (pos + ag_SAMPLES_PER_SYMBOL - 4 + j) % ag_SAMPLES_PER_SYMBOL;
						avgBuffer[nPos] = avgValues[j];
					}
				}

				int maxPosition = -1;
				float maxValue = 0.f;

				for (int j = 0; j < ag_SAMPLES_PER_SYMBOL; j++)
				{
					if (maxValue < avgBuffer[j])
					{
						maxValue = avgBuffer[j];
						maxPosition = j;
					}
				}

				if (Depacketizer_Stable_Connection() == 1)
					_ClockRecovery_averagePosition = maxPosition;

			}
		}

		if (_ClockRecovery_averagePosition == (_ClockRecovery_lastBufferPosition + ag_SAMPLES_PER_SYMBOL / 2) % ag_SAMPLES_PER_SYMBOL)
		{
			float average = 0.f;
			for (int j = 0; j < ag_SAMPLES_PER_SYMBOL; j++)
			{
				average += _ClockRecovery_lastBuffer[j];
			}

			ret.data[numBits++] = average;
		}

		_ClockRecovery_samplesSinceLastBit++;
	}

	ret.count = numBits;

	PRINT_TIMER("ClockRecovery_OnData");

	ClockRecovery_ReportData(ret);
	free(ret.data);
}

void ClockRecovery_Init()
{
	_lastValues = (float *)calloc(ag_SAMPLES_PER_SYMBOL, sizeof(float));
	_lastAverages = (float *)calloc(ag_SAMPLES_PER_SYMBOL, sizeof(float));

}