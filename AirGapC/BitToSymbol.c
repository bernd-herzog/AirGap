#include "BitToSymbol.h"
#include "DataTypes.h"
#include <stdlib.h>

extern void(*BitToSymbol_ReportData)(ComplexPackage);
extern void BitToSymbol_OnData(BoolPackage);

void BitToSymbol_OnData(BoolPackage packet)
{
	ComplexPackage ret;
	ret.count = packet.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));

	//TODO: jedes bit in -1.0 oder 1.0 umwandeln

	BitToSymbol_ReportData(ret);
	free(ret.data);
}
