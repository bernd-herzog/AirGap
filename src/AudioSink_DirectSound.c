#include "AudioSink.h"
#include "DataTypes.h"

#include <stdbool.h>
#include <stdio.h>
#include <dsound.h>

#include "agmath.h"

#include <time.h>

#define numbuffers 8
#define BUFFER_SIZE (ag_SAMPLERATE*8)

extern void AudioSink_OnData(ComplexPackage);

void Init();
void WaitForFreeBuffer();
void WriteToBuffer(Complex *, int);

LPDIRECTSOUNDBUFFER soundBuffer = 0;
HANDLE event;
DWORD ourPosition = 0;

void AudioSink_OnData(ComplexPackage complexPackage)
{
	printf("AudioSink Got %d samples (worth %f s of audio)\n", complexPackage.count, (float)complexPackage.count/ag_SAMPLERATE);

	if (soundBuffer == 0)
	{
		Init();
	}

	DWORD positionInBuffer = ourPosition % (BUFFER_SIZE / numbuffers);

	int bytesWritten = 0;

	if (positionInBuffer != 0)
	{
		WaitForFreeBuffer();

		//write rest of buffer
		int bytesInBufferLeft = (BUFFER_SIZE / numbuffers) - positionInBuffer;

		if (complexPackage.count * 2 < bytesInBufferLeft)
		{
			WriteToBuffer(complexPackage.data, complexPackage.count);
			return;
		}
		else
		{
			WriteToBuffer(complexPackage.data, bytesInBufferLeft /2);
			bytesWritten = bytesInBufferLeft;
		}
	}

	while (complexPackage.count * 2 - bytesWritten > (BUFFER_SIZE / numbuffers))
	{
		WaitForFreeBuffer();

		WriteToBuffer(complexPackage.data + (bytesWritten / 2), (BUFFER_SIZE / numbuffers) /2);

		bytesWritten += (BUFFER_SIZE / numbuffers);
	}

	if (complexPackage.count * 2 > bytesWritten)
	{
		WaitForFreeBuffer();


		WriteToBuffer(complexPackage.data + (bytesWritten / 2), complexPackage.count - bytesWritten / 2);
	}
}

void Init()
{
	LPDIRECTSOUND directSound;
	HRESULT result = DirectSoundCreate(NULL, &directSound, NULL);

	result = directSound->lpVtbl->SetCooperativeLevel(directSound, GetDesktopWindow(), DSSCL_EXCLUSIVE);

	WAVEFORMATEX waveformat;
	memset(&waveformat, 0, sizeof(WAVEFORMATEX));
	waveformat.cbSize = sizeof(WAVEFORMATEX);

	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nSamplesPerSec = ag_SAMPLERATE;
	waveformat.nChannels = 1;
	waveformat.wBitsPerSample = 16;

	waveformat.nBlockAlign = (short)(waveformat.nChannels * waveformat.wBitsPerSample / 8);
	waveformat.nAvgBytesPerSec = waveformat.nSamplesPerSec * waveformat.nBlockAlign;

	DSBUFFERDESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(DSBUFFERDESC));
	bufferDescription.dwSize = sizeof(DSBUFFERDESC);
	bufferDescription.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
	bufferDescription.dwBufferBytes = BUFFER_SIZE;
	bufferDescription.lpwfxFormat = &waveformat;

	result = directSound->lpVtbl->CreateSoundBuffer(directSound, &bufferDescription, &soundBuffer, NULL);

	LPDIRECTSOUNDNOTIFY notify;

	const IID *iid = &IID_IDirectSoundNotify;

	result = soundBuffer->lpVtbl->QueryInterface(soundBuffer, iid, (VOID**)&notify);

	event = CreateEvent(NULL, FALSE, FALSE, NULL);


	DSBPOSITIONNOTIFY positionNotify[numbuffers];

	for (int i = 0; i < numbuffers; i++)
	{
		positionNotify[i].dwOffset = (BUFFER_SIZE / numbuffers * (i + 1)) - 1;
		positionNotify[i].hEventNotify = event;
	}

	LPVOID buffer = 0;
	DWORD bufferLen = 0;
	LPVOID buffer2 = 0;
	DWORD bufferLen2 = 0;

	result = soundBuffer->lpVtbl->Lock(soundBuffer, 0, BUFFER_SIZE, &buffer, &bufferLen, &buffer2, &bufferLen2, 0);

	short *samples = (short *)buffer;

	ZeroMemory(samples, BUFFER_SIZE);

	result = soundBuffer->lpVtbl->Unlock(soundBuffer, buffer, bufferLen, buffer2, bufferLen2);
	result = notify->lpVtbl->SetNotificationPositions(notify, numbuffers, positionNotify);
	result = soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);
}

void WaitForFreeBuffer()
{
	while (true)
	{
		DWORD playPosition, writePosition;
		
		//do we have to wait?
		HRESULT result = soundBuffer->lpVtbl->GetCurrentPosition(soundBuffer, &playPosition, &writePosition);

		DWORD part = writePosition % (BUFFER_SIZE / numbuffers);
		DWORD buffer = writePosition / (BUFFER_SIZE / numbuffers);

		DWORD ourBuffer = ourPosition / (BUFFER_SIZE / numbuffers);

		printf("Wait: write in %d, read in %d\n", ourBuffer, buffer);

		if (ourBuffer == buffer)
		{
			clock_t waitdurstart, waitdurend;
			float duration;
			
			waitdurstart = clock();

			DWORD dwResult = MsgWaitForMultipleObjects(1, &event, FALSE, INFINITE, QS_ALLEVENTS);

			waitdurend = clock();
			duration = ((float)(waitdurend - waitdurstart)) / CLOCKS_PER_SEC;
			printf("AudioSink waited %f s\n", duration);

			if (dwResult != S_OK)
				return;
		}
		else
		{
			break;
		}
	}
}

void WriteToBuffer(Complex *source, int num)
{
	LPVOID buffer = 0;
	DWORD bufferLen = 0;
	LPVOID buffer2 = 0;
	DWORD bufferLen2 = 0;

	HRESULT result = soundBuffer->lpVtbl->Lock(soundBuffer, ourPosition, num*2, &buffer, &bufferLen, &buffer2, &bufferLen2, 0);

	if (bufferLen2 != 0)
		return;

	short *samples = (short *)buffer;

	const float gain = 0.99f;

	for (int i = 0; i < num; i++)
	{
		float gValue = source[i].i * gain;
		if (gValue < -1.0f || gValue > 1.0f){
			return;
		}

		samples[i] = (short)(gValue * (float)0x8000);
	}

	result = soundBuffer->lpVtbl->Unlock(soundBuffer, buffer, bufferLen, buffer2, bufferLen2);

	ourPosition += num * 2;
	ourPosition = ourPosition % BUFFER_SIZE;
}
