#include "BinarySlicer.h"
#include "DataTypes.h"
#include <stdbool.h>
#include <stdlib.h>
#include "agmath.h"

extern void(*BinarySlicer_ReportData)(BoolPackage);
extern void BinarySlicer_OnData(FloatPackage);

void BinarySlicer_OnData(FloatPackage packet)
{
	START_TIMER;

	BoolPackage ret;
	ret.count = packet.count;
	ret.data = (bool *)malloc(ret.count * sizeof(bool));

	for (int i = 0; i < ret.count; i++)
	{
		if (packet.data[i] < 0.f){
			ret.data[i] = false;
		}
		else{
			ret.data[i] = true;
		}
	}

	PRINT_TIMER("BinarySlicer_OnData");

	BinarySlicer_ReportData(ret);
	free(ret.data);
}
