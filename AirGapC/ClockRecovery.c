#include "ClockRecovery.h"

extern void(*ClockRecovery_ReportData)(short *data);
extern void ClockRecovery_OnData(float *data);

void ClockRecovery_OnData(float *data)
{
	//do something with data
	ClockRecovery_ReportData((short *)data);
}
