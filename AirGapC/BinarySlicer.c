#include "BinarySlicer.h"

extern void(*BinarySlicer_ReportData)(short *data);
extern void BinarySlicer_OnData(short *data);

void BinarySlicer_OnData(short *data)
{
	//do something with data
	BinarySlicer_ReportData(data);
}
