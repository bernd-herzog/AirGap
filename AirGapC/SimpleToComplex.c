#include "SimpleToComplex.h"

extern void(*SimpleToComplex_ReportData)(short *data);
extern void SimpleToComplex_OnData(short *data);

void SimpleToComplex_OnData(short *data)
{
	//do something with data
	SimpleToComplex_ReportData(data);
}
