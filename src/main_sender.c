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

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.

#include <windows.h>
#include <tchar.h>

#include "main.h"

void AirGap_main();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	AirGap_main();

	return 0;
}

int main(int argc, char **argv)
{
	AirGap_main();

	return 0;
}

void AirGap_main()
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
