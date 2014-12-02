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

	for (int i = 0; i < ret.count; i++)
	{
		bool value = packet.data[i];
		if (value == true){
			ret.data[i].i = 1.f;
		}
		else{
			ret.data[i].i = -1.f;
		}
		
	}

	BitToSymbol_ReportData(ret);
	free(ret.data);
}
