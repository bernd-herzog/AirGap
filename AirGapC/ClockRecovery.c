#include "ClockRecovery.h"

extern void(*ClockRecovery_ReportData)(short *data);
extern void ClockRecovery_OnData(short *data);

void ClockRecovery_OnData(short *data)
{
	//do something with data
	ClockRecovery_ReportData(data);
}
