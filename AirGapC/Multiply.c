#include "Multiply.h"

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);

void Multiply_OnData(ComplexPackage data)
{
	//do something with data
	Multiply_ReportData(data);
}
