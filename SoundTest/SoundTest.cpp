// SoundTest.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <dsound.h>


int _tmain(int argc, _TCHAR* argv[])
{
	WAVEFORMATEX waveformat;
	memset(&waveformat, 0, sizeof(WAVEFORMATEX));
	waveformat.cbSize = sizeof(WAVEFORMATEX);

	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nSamplesPerSec = 44100;
	waveformat.nChannels = 1;
	waveformat.wBitsPerSample = 16;

	waveformat.nBlockAlign = (short)(waveformat.nChannels * waveformat.wBitsPerSample / 8);
	waveformat.nAvgBytesPerSec = waveformat.nSamplesPerSec * waveformat.nBlockAlign;

	LPDIRECTSOUNDCAPTURE8 lpDSC;
	DirectSoundCaptureCreate(NULL, &lpDSC, NULL);
	
	DSCBUFFERDESC bufferDescription;
	memset(&bufferDescription, 0, sizeof(DSCBUFFERDESC));
	bufferDescription.dwSize = sizeof(DSCBUFFERDESC);
	bufferDescription.dwFlags = DSCBCAPS_WAVEMAPPED;
	bufferDescription.dwBufferBytes = 512;

	bufferDescription.lpwfxFormat = &waveformat;
	bufferDescription.dwFXCount = 0;
		
	LPDIRECTSOUNDCAPTUREBUFFER captureBuffer;
	HRESULT result = lpDSC->CreateCaptureBuffer(&bufferDescription, &captureBuffer, NULL);
	captureBuffer->Start(DSCBSTART_LOOPING);

	Sleep(50);

	void *pData1;
	void *pData2;

	DWORD len1, len2;

	captureBuffer->Lock(0, 128, &pData1, &len1, &pData2, &len2, 0L);

	return 0;
}

