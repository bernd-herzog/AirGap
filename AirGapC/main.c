#include "main.h"

#include "AudioSource.h"
#include "AudioSink.h"

#include "FileSink.h"
#include "FileSource.h"

#include "BinarySlicer.h"
#include "BitToSymbol.h"

#include "FrequencyModulator.h"
#include "QuadraturDemodulator.h"

#include "Multiply.h"
#include "ClockRecovery.h"
#include "FirFilter.h"

#include <stdbool.h>

void AirGap_main()
{
	bool isReceiver = false;

	if (isReceiver){
		FirFilter_InitLowPass();

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
		FirFilter_InitGaussian();

		CONNECT(FileSource, BitToSymbol);
		CONNECT(BitToSymbol, FirFilter);
		CONNECT(FirFilter, FrequencyModulator);
		CONNECT(FrequencyModulator, Multiply);
		CONNECT(Multiply, AudioSink);

		FileSource_Work();
	}
}