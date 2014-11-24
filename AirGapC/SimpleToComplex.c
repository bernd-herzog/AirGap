#include "SimpleToComplex.h"

extern void(*SimpleToComplex_ReportData)(Complex *);
extern void(SimpleToComplex_OnData)(short *data);

void(SimpleToComplex_OnData)(short *data)
{
	//do something with data

	Complex *p = (Complex *)data;

	BandPassFilter_ReportData(p);
}
