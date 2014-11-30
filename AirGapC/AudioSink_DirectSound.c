#include "AudioSink.h"
#include "DataTypes.h"

#include <stdbool.h>
#include <stdio.h>
#include <dsound.h>
#include <math.h>

#define BUFFER_SIZE (1024*128)
#define numbuffers 8

extern void AudioSink_OnData(ComplexPackage);

void Init();
void WaitForFreeBuffer();
void WriteToBuffer(Complex *source, int num);

LPDIRECTSOUNDBUFFER soundBuffer = 0;
HANDLE event;
DWORD ourPosition = 0;

void AudioSink_OnData(ComplexPackage data)
{
	if (soundBuffer == 0)
	{
		Init();
	}

	DWORD positionInBuffer = ourPosition % (BUFFER_SIZE / numbuffers);

	int written = 0;

	if (positionInBuffer != 0)
	{
		WaitForFreeBuffer();

		//write rest of buffer
		int rest = (BUFFER_SIZE / numbuffers) - positionInBuffer;

		if (data.count*2 < rest)
		{
			rest = data.count * 2;
			WriteToBuffer(data.data, rest);
			return;
		}
		else
		{
			WriteToBuffer(data.data, rest);
			written = rest;
		}
	}

	while (data.count * 2 - written > (BUFFER_SIZE / numbuffers))
	{
		WaitForFreeBuffer();
		WriteToBuffer(data.data + written, (BUFFER_SIZE / numbuffers));

		written += (BUFFER_SIZE / numbuffers);
	}

	if (data.count * 2 > written)
	{
		WaitForFreeBuffer();
		WriteToBuffer(data.data + written, data.count * 2 - written);
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
	waveformat.nSamplesPerSec = 44100;
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

		DWORD ourBuffer = ourPosition / BUFFER_SIZE;

		if (ourBuffer == buffer)
		{
			DWORD dwResult = MsgWaitForMultipleObjects(1, &event, FALSE, INFINITE, QS_ALLEVENTS);
		}
		else
			break;
	}
}

void WriteToBuffer(Complex *source, int num)
{
	LPVOID buffer = 0;
	DWORD bufferLen = 0;
	LPVOID buffer2 = 0;
	DWORD bufferLen2 = 0;

	HRESULT result = soundBuffer->lpVtbl->Lock(soundBuffer, ourPosition, num * 2, &buffer, &bufferLen, &buffer2, &bufferLen2, 0);

	short *samples = (short *)buffer;

	for (int i = 0; i < num / 2; i++)
	{
		samples[i] = (short)(source[i].i * (float)0x8000);
	}

	result = soundBuffer->lpVtbl->Unlock(soundBuffer, buffer, bufferLen, buffer2, bufferLen2);

	ourPosition += num * 2;
	ourPosition = ourPosition % BUFFER_SIZE;
}
