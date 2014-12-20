#include "AudioSource.h"
#include "DataTypes.h"
#include "agmath.h"

#include <stdbool.h>
#include <stdio.h>
#include <dsound.h>

#include <time.h>


#define BUFFER_SIZE (1024*128)
#define numbuffers 8

extern void(*AudioSource_ReportData)(ComplexPackage);
extern void AudioSource_Work();

void InitInput();
void RunLoop();
ComplexPackage ReadData();
bool WaitForData();
void AudioSource_DS_WaitForFreeBuffer();
DWORD _ourPosition = 0;


HANDLE g_event;
LPDIRECTSOUNDCAPTUREBUFFER g_captureBuffer;

void AudioSource_Work()
{
	InitInput();
	RunLoop();
}

void InitInput()
{
	LPDIRECTSOUNDCAPTURE capture;
	HRESULT result = DirectSoundCaptureCreate(NULL, &capture, NULL);

	WAVEFORMATEX waveformat;
	memset(&waveformat, 0, sizeof(WAVEFORMATEX));
	waveformat.cbSize = sizeof(WAVEFORMATEX);

	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nSamplesPerSec = ag_SAMPLERATE;
	waveformat.nChannels = 1;
	waveformat.wBitsPerSample = 16;

	waveformat.nBlockAlign = (short)(waveformat.nChannels * waveformat.wBitsPerSample / 8);
	waveformat.nAvgBytesPerSec = waveformat.nSamplesPerSec * waveformat.nBlockAlign;

	DSCBUFFERDESC bufferDescription;
	memset(&bufferDescription, 0, sizeof(DSCBUFFERDESC));
	bufferDescription.dwSize = sizeof(DSCBUFFERDESC);
	bufferDescription.dwFlags = DSCBCAPS_WAVEMAPPED;
	bufferDescription.dwBufferBytes = BUFFER_SIZE;

	bufferDescription.lpwfxFormat = &waveformat;
	bufferDescription.dwFXCount = 0;

	result = capture->lpVtbl->CreateCaptureBuffer(capture, &bufferDescription, &g_captureBuffer, NULL);

	void *pData1 = 0, *pData2 = 0;

	DWORD len1 = 0, len2 = 0;

	LPDIRECTSOUNDNOTIFY8 notify;

	const IID *iid = &IID_IDirectSoundNotify;

	result = g_captureBuffer->lpVtbl->QueryInterface(g_captureBuffer, iid, (VOID**)&notify);

	g_event = CreateEvent(NULL, FALSE, FALSE, NULL);


	DSBPOSITIONNOTIFY positionNotify[numbuffers];

	for (int i = 0; i < numbuffers; i++)
	{
		positionNotify[i].dwOffset = (BUFFER_SIZE / numbuffers * (i + 1)) - 1;
		positionNotify[i].hEventNotify = g_event;

	}


	result = notify->lpVtbl->SetNotificationPositions(notify, numbuffers, positionNotify);

	g_captureBuffer->lpVtbl->Start(g_captureBuffer, DSCBSTART_LOOPING);
}

void RunLoop()
{
	while (true)
	{
		//bool hasData = WaitForData();

		//if (hasData)
		{
			AudioSource_DS_WaitForFreeBuffer();

			ComplexPackage data = ReadData();

			AudioSource_ReportData(data);

			free(data.data);
		}
	}
}

bool WaitForData()
{
	DWORD dwResult = MsgWaitForMultipleObjects(1, &g_event, FALSE, INFINITE, QS_ALLEVENTS);
	return dwResult == WAIT_OBJECT_0;
}

ComplexPackage ReadData()
{
	static int caputureOffset = 0;

	DWORD capturePosition = 0, readPosition = 0;

	HRESULT res = g_captureBuffer->lpVtbl->GetCurrentPosition(g_captureBuffer, &capturePosition, &readPosition);

	//int lockSize = readPosition - caputureOffset;
	//if (lockSize < 0)
	//	lockSize += BUFFER_SIZE;

	//lockSize -= (lockSize % (BUFFER_SIZE / numbuffers));

	caputureOffset = readPosition - readPosition % (BUFFER_SIZE / numbuffers) - (BUFFER_SIZE / numbuffers);
	if (caputureOffset < 0)
		caputureOffset += BUFFER_SIZE;

	void *pData1, *pData2;
	DWORD len1, len2;

	res = g_captureBuffer->lpVtbl->Lock(g_captureBuffer, caputureOffset, (BUFFER_SIZE / numbuffers), &pData1, &len1, &pData2, &len2, 0L);

	int countSamples = len1 / 2;

	ComplexPackage ret;
	ret.count = countSamples;
	ret.data = (Complex *)malloc(countSamples * sizeof(Complex));

	short *bufferData = (short *)pData1;

	for (int i = 0; i < countSamples; i++)
	{
		float sampleValue = ((float)bufferData[i] / 0x8000);

		ret.data[i].i = sampleValue;
		ret.data[i].q =  sampleValue;
	}

	if (pData1)
	{
		res = g_captureBuffer->lpVtbl->Unlock(g_captureBuffer, pData1, len1, pData2, len2);
	}

	//TODO: remove caputureOffset
	caputureOffset += len1;
	_ourPosition += len1;

	caputureOffset %= BUFFER_SIZE;
	_ourPosition %= BUFFER_SIZE;

	return ret;
}

void AudioSource_DS_WaitForFreeBuffer()
{
	while (true)
	{
		DWORD playPosition, writePosition;

		//do we have to wait?
		HRESULT result = g_captureBuffer->lpVtbl->GetCurrentPosition(g_captureBuffer, &playPosition, &writePosition);

		DWORD part = writePosition % (BUFFER_SIZE / numbuffers);
		DWORD buffer = writePosition / (BUFFER_SIZE / numbuffers);

		DWORD ourBuffer = _ourPosition / (BUFFER_SIZE / numbuffers);

		//printf("Wait: read in %d, read in %d\n", ourBuffer, buffer);

		if (ourBuffer == buffer)
		{
			clock_t waitdurstart, waitdurend;
			float duration;

			waitdurstart = clock();

			DWORD dwResult = MsgWaitForMultipleObjects(1, &g_event, FALSE, INFINITE, QS_ALLEVENTS);

			waitdurend = clock();
			duration = ((float)(waitdurend - waitdurstart)) / CLOCKS_PER_SEC;
			//printf("AudioSource waited %f s\n", duration);

			if (dwResult != S_OK)
				return;
		}
		else
		{
			break;
		}
	}
}