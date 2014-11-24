#include "BandPassFilter.h"

extern void(*BandPassFilter_ReportData)(short *);
extern void(BandPassFilter_OnData)(short *data);

void(BandPassFilter_OnData)(short *data)
{
	//do something with data

	BandPassFilter_ReportData(data);
}
