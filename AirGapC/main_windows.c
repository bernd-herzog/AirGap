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


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

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
