#include "QuadraturDemodulator.h"

extern void(*QuadraturDemodulator_ReportData)(short *data);
extern void QuadraturDemodulator_OnData(short *data);

void QuadraturDemodulator_OnData(short *data)
{
	//do something with data
	QuadraturDemodulator_ReportData(data);
}
