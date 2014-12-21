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


#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>
	#include <tchar.h>
	#include <SDKDDKVer.h>
#endif

#include "main.h"

void AirGap_main();


#ifdef WIN32
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
#endif

int main(int argc, char **argv)
{
	AirGap_main();

	return 0;
}

void AirGap_main()
{
	FirFilter_InitLowPass();
	Multiply_SetFrequency(-ag_BASE_FREQUENCY);
	ClockRecovery_Init();

	CONNECT(AudioSource, Multiply);
	CONNECT(Multiply, FirFilter);
	CONNECT(FirFilter, QuadraturDemodulator);
	CONNECT(QuadraturDemodulator, ClockRecovery);
	CONNECT(ClockRecovery, BinarySlicer);
	CONNECT(BinarySlicer, FileSink);

	AudioSource_Work();
}
