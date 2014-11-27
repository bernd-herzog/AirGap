// SoundTest.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdio.h>

#include <dsound.h>

#define BUFFER_SIZE (1024*128)
#define numbuffers 8


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
	bufferDescription.dwBufferBytes = BUFFER_SIZE;

	bufferDescription.lpwfxFormat = &waveformat;
	bufferDescription.dwFXCount = 0;

	LPDIRECTSOUNDCAPTUREBUFFER captureBuffer;
	HRESULT result = capture->CreateCaptureBuffer(&bufferDescription, &captureBuffer, NULL);

	Sleep(50);

	void *pData1 = 0;
	void *pData2;

	DWORD len1, len2;

	LPDIRECTSOUNDNOTIFY8 notify;
	captureBuffer->QueryInterface(IID_IDirectSoundNotify, (VOID**)&notify);

	HANDLE event = CreateEvent(NULL, FALSE, FALSE, NULL);


	DSBPOSITIONNOTIFY positionNotify[numbuffers];

	for (int i = 0; i < numbuffers; i++)
	{
		positionNotify[i].dwOffset = (BUFFER_SIZE / numbuffers * (i + 1)) - 1;
		positionNotify[i].hEventNotify = event;

	}


	notify->SetNotificationPositions(numbuffers , positionNotify);


	captureBuffer->Start(DSCBSTART_LOOPING);

	int i = 0;
	int caputureOffset = 0;
	while (true)
	{
		DWORD dwResult = MsgWaitForMultipleObjects(1, &event, FALSE, INFINITE, QS_ALLEVENTS);
		if (dwResult == WAIT_OBJECT_0)
		{
			DWORD capturePosition = 0, readPosition = 0;

			captureBuffer->GetCurrentPosition(&capturePosition, &readPosition);

			int lockSize = readPosition - caputureOffset;
			if (lockSize < 0)
				lockSize += BUFFER_SIZE;

			lockSize -= (lockSize % (BUFFER_SIZE / numbuffers));

			caputureOffset = readPosition - readPosition % (BUFFER_SIZE / numbuffers) - (BUFFER_SIZE / numbuffers);
			if (caputureOffset < 0)
				caputureOffset += BUFFER_SIZE;

			HRESULT res = captureBuffer->Lock(caputureOffset, lockSize, &pData1, &len1, &pData2, &len2, 0L);

			if (pData1)
			{
				printf("%d\t%d\t%d\n", i, caputureOffset, lockSize);
			}
			else{
			}
			res = captureBuffer->Unlock(pData1, len1, pData2, len2);

			caputureOffset += len1;
			caputureOffset %= BUFFER_SIZE; // Circular buffer


		}
		i++;
	}


	return 0;
}
