#include "BinarySlicer.h"
#include "DataTypes.h"
#include <stdbool.h>
#include <stdlib.h>

extern void(*BinarySlicer_ReportData)(BoolPackage);
extern void BinarySlicer_OnData(FloatPackage);

void BinarySlicer_OnData(FloatPackage packet)
{
	BoolPackage ret;
	ret.count = packet.count;
	ret.data = (bool *)malloc(ret.count * sizeof(bool));

	//TODO: jedes float in false oder true umwandeln

	BinarySlicer_ReportData(ret);
	free(ret.data);
}
