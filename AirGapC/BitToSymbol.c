#include "BitToSymbol.h"
#include "DataTypes.h"

extern void(*BitToSymbol_ReportData)(ComplexPackage);
extern void BitToSymbol_OnData(BoolPackage);

void BitToSymbol_OnData(BoolPackage packet)
{
	ComplexPackage ret;
	ret.count = 0;
	ret.data = 0;
	//do something with data
	BitToSymbol_ReportData(ret);
}
