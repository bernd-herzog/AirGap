#include "ClockRecovery.h"
#include "DataTypes.h"

extern void(*ClockRecovery_ReportData)(FloatPackage);
extern void ClockRecovery_OnData(FloatPackage);

void ClockRecovery_OnData(FloatPackage packet)
{
	//do something with data
	ClockRecovery_ReportData(packet);
}
