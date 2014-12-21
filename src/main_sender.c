#include "main.h"
#include "AudioSink.h"
#include "FileSource.h"
#include "BitToSymbol.h"
#include "FrequencyModulator.h"
#include "Repeater.h"
#include "Multiply.h"
#include "FirFilter.h"
#include "Packetizer.h"

#include <stdbool.h>
#include "agmath.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <SDKDDKVer.h>
	#include <windows.h>
	#include <tchar.h>
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
	FirFilter_InitGaussian();
	Multiply_SetFrequency(ag_BASE_FREQUENCY);
	Packetizer_Init();

	CONNECT(FileSource, Packetizer);
	CONNECT(Packetizer, BitToSymbol);
	CONNECT(BitToSymbol, Repeater);
	CONNECT(Repeater, FirFilter);
	CONNECT(FirFilter, FrequencyModulator);
	CONNECT(FrequencyModulator, Multiply);
	CONNECT(Multiply, AudioSink);

	FileSource_Work();
}
