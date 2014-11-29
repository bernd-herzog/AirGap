#include "BandPassFilter.h"
#include "DataTypes.h"

extern void(*BandPassFilter_ReportData)(ComplexPackage);
extern void BandPassFilter_OnData(ComplexPackage);

void BandPassFilter_OnData(ComplexPackage packet)
{
	//Nicht implementieren!
	BandPassFilter_ReportData(packet);
}
