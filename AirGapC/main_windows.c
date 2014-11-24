#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "AudioSource.h"
#include "BandPassFilter.h"
#include "SimpleToComplex.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	BOOL isReceiver = TRUE;

	if (isReceiver){
		AudioSource_ReportData = &BandPassFilter_OnData;
		BandPassFilter_ReportData = &SimpleToComplex_OnData;
	}

	AudioSource_Work();

	return 0;
}
