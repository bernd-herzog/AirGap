#include "BitToSymbol.h"
#include "DataTypes.h"

extern void(*BitToSymbol_ReportData)(FloatPackage);
extern void BitToSymbol_OnData(BoolPackage);

void BitToSymbol_OnData(BoolPackage packet)
{
	FloatPackage ret;
	//do something with data
	BitToSymbol_ReportData(ret);
}
