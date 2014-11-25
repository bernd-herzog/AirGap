#include "BitToSymbol.h"

extern void(*BitToSymbol_ReportData)(short *data);
extern void BitToSymbol_OnData(short *data);

void BitToSymbol_OnData(short *data)
{
	//do something with data
	BitToSymbol_ReportData(data);
}
