#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#define CONNECT(source ,target) source ## _ ## ReportData = &target ## _ ## OnData

#include "AudioSource.h"
#include "BandPassFilter.h"
#include "SimpleToComplex.h"
#include "Multiply.h"
#include "QuadraturDemodulator.h"
#include "ClockRecovery.h"
#include "BinarySlicer.h"
#include "FileSink.h"
#include "FileSource.h"
#include "BitToSymbol.h"
#include "FirFilter.h"
#include "FrequencyModulator.h"
#include "ComplexToSimple.h"
#include "AudioSink.h"

int main(int argc, char **argv)
{
	bool isReceiver = true;

	if (isReceiver){
		CONNECT(AudioSource, BandPassFilter);
		CONNECT(BandPassFilter, SimpleToComplex);
		CONNECT(SimpleToComplex, Multiply);
		CONNECT(Multiply, QuadraturDemodulator);
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
		CONNECT(Multiply, ComplexToSimple);
		CONNECT(ComplexToSimple, AudioSink);

		FileSource_Work();
	}


	return 0;
}
