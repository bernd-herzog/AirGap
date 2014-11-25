#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.

#define CONNECT(source ,target) source ## _ ## ReportData = &target ## _ ## OnData

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdbool.h>


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

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

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
