#include "BandPassFilter.h"
#include "DataTypes.h"

extern void(*BandPassFilter_ReportData)(ComplexPackage);
extern void BandPassFilter_OnData(ComplexPackage);

void BandPassFilter_OnData(ComplexPackage packet)
{
	//do something with data
	BandPassFilter_ReportData(packet);
}
