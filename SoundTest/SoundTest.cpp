// SoundTest.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdio.h>

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

	LPDIRECTSOUNDCAPTURE8 capture;
	DirectSoundCaptureCreate(NULL, &capture, NULL);

	DSCBUFFERDESC bufferDescription;
	memset(&bufferDescription, 0, sizeof(DSCBUFFERDESC));
	bufferDescription.dwSize = sizeof(DSCBUFFERDESC);
	bufferDescription.dwFlags = DSCBCAPS_WAVEMAPPED;
	bufferDescription.dwBufferBytes = 512;

	bufferDescription.lpwfxFormat = &waveformat;
	bufferDescription.dwFXCount = 0;

	LPDIRECTSOUNDCAPTUREBUFFER captureBuffer;
	HRESULT result = capture->CreateCaptureBuffer(&bufferDescription, &captureBuffer, NULL);

	Sleep(50);

	void *pData1;
	void *pData2;

	DWORD len1, len2;

	LPDIRECTSOUNDNOTIFY8 notify;
	captureBuffer->QueryInterface(IID_IDirectSoundNotify, (VOID**)&notify);

	HANDLE event = CreateEvent(NULL, FALSE, FALSE, NULL);

	DSBPOSITIONNOTIFY positionNotify[2];
	positionNotify[0].dwOffset = 0;
	positionNotify[0].hEventNotify = event;

	positionNotify[1].dwOffset = 256;
	positionNotify[1].hEventNotify = event;


	notify->SetNotificationPositions(2, positionNotify);


	captureBuffer->Start(DSCBSTART_LOOPING);

	
	while (true)
	{
		DWORD dwResult = MsgWaitForMultipleObjects(1, &event, FALSE, INFINITE, QS_ALLEVENTS);
		if (dwResult == WAIT_OBJECT_0)
		{
			captureBuffer->Lock(0, 128, &pData1, &len1, &pData2, &len2, 0L);
			//printf("%X %X %X %X\n", ((char *)pData1)[0], ((char *)pData1)[1], ((char *)pData1)[2], ((char *)pData1)[3]);
			captureBuffer->Unlock(&pData1, len1, &pData2, len2);
		}

	}


	return 0;
}
