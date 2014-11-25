#include "FrequencyModulator.h"

extern void(*FrequencyModulator_ReportData)(short *data);
extern void FrequencyModulator_OnData(short *data);

void FrequencyModulator_OnData(short *data)
{
	//do something with data
	FrequencyModulator_ReportData(data);
}
