#include "AudioSource.h"

extern void(*AudioSource_ReportData)(short *);
extern void AudioSource_Work();


void AudioSource_Work()
{
	short someSamples[512];

	AudioSource_ReportData(someSamples);

	return;
}
