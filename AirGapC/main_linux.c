#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>

#define CONNECT(source ,target) source ## _ ## ReportData = &target ## _ ## OnData

#include "AudioSource.h"
#include "BandPassFilter.h"
#include "Multiply.h"
#include "QuadraturDemodulator.h"
#include "ClockRecovery.h"
#include "BinarySlicer.h"
#include "FileSink.h"
#include "FileSource.h"
#include "BitToSymbol.h"
#include "FirFilter.h"
#include "FrequencyModulator.h"
#include "AudioSink.h"

int main(int argc, char **argv)
{
	bool isReceiver = false;

	if (isReceiver){
		CONNECT(AudioSource, Multiply);
		CONNECT(Multiply, FirFilter);
		CONNECT(FirFilter, QuadraturDemodulator);
		CONNECT(QuadraturDemodulator, ClockRecovery);
		CONNECT(ClockRecovery, BinarySlicer);
		CONNECT(BinarySlicer, FileSink);

		AudioSource_Work();
	}
	else
	{
		CONNECT(FileSource, BitToSymbol);
		CONNECT(BitToSymbol, FirFilter);
		CONNECT(FirFilter, FrequencyModulator);
		CONNECT(FrequencyModulator, Multiply);
		CONNECT(Multiply, AudioSink);

		FileSource_Work();
	}


	return 0;
}
