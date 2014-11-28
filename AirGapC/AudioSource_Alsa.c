#include "AudioSource.h"

extern void(*AudioSource_ReportData)(short *);
extern void AudioSource_Work();

void AudioSource_Work()
{
	InitInput();
	RunLoop();
}
