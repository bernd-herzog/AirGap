#include "BinarySlicer.h"
#include "DataTypes.h"

extern void(*BinarySlicer_ReportData)(BoolPackage);
extern void BinarySlicer_OnData(FloatPackage);

void BinarySlicer_OnData(FloatPackage packet)
{
	BoolPackage ret;
	//do something with data
	BinarySlicer_ReportData(ret);
}
