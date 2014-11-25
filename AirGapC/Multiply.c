#include "Multiply.h"

extern void(*Multiply_ReportData)(short *data);
extern void Multiply_OnData(short *data);

void Multiply_OnData(short *data)
{
	//do something with data
	Multiply_ReportData(data);
}
