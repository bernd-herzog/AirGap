#include "main.h"

#include "AudioSource.h"
#include "AudioSink.h"

#include "FileSink.h"
#include "FileSource.h"

#include "BinarySlicer.h"
#include "BitToSymbol.h"

#include "FrequencyModulator.h"
#include "QuadraturDemodulator.h"

#include "ClockRecovery.h"
#include "Repeater.h"

#include "Multiply.h"
#include "FirFilter.h"

#include <stdbool.h>
#include "agmath.h"

void AirGap_main()
{
	bool isReceiver = false;

	if (isReceiver){
		FirFilter_InitLowPass();
		Multiply_SetFrequency(-ag_BASE_FREQUENCY);

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
		Multiply_SetFrequency(ag_BASE_FREQUENCY);

		CONNECT(FileSource, BitToSymbol);
		CONNECT(BitToSymbol, Repeater);
		CONNECT(Repeater, FirFilter);
		CONNECT(FirFilter, FrequencyModulator);
		CONNECT(FrequencyModulator, Multiply);
		CONNECT(Multiply, AudioSink);

		FileSource_Work();
	}
}