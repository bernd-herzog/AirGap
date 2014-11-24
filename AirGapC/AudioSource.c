#include "AudioSource.h"

extern void(*AudioSource_ReportData)(short *);



void work()
{
	short someSamples[512];

	AudioSource_ReportData(someSamples);

	return;
}
