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
LPDIRECTSOUNDBUFFER soundBuffer = 0;


void AudioSink_OnData(ComplexPackage data)
{
	if (soundBuffer == 0)
		Init();

	//do something with data

	void *buffer = 0;
	DWORD bufferLen = 0;
	LPVOID buffer2 = 0;
	DWORD bufferLen2 = 0;

	DWORD p1, p2;

	HRESULT result = soundBuffer->lpVtbl->GetCurrentPosition(soundBuffer, &p1, &p2);

	//TODO: wait if buffer is full

	result = soundBuffer->lpVtbl->Lock(soundBuffer, 0, BUFFER_SIZE, &buffer, &bufferLen, &buffer2, &bufferLen2, 0);

	short *samples = (short *)buffer;

	for (int i = 0; i < BUFFER_SIZE / 2; i++)
	{
		float x = (float)i;
		x *= 2 * 3.1415f;
		x /= 22;
		float y = sinf(x);

		samples[i] = 0;
	}

	result = soundBuffer->lpVtbl->Unlock(soundBuffer, buffer, bufferLen, buffer2, bufferLen2);



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

	HANDLE event = CreateEvent(NULL, FALSE, FALSE, NULL);


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

	for (int i = 0; i < BUFFER_SIZE / 2; i++)
	{
		float x = (float)i;
		x *= 2 * 3.1415f;
		x /= 22;
		float y = sinf(x);

		samples[i] = 0;
	}

	result = soundBuffer->lpVtbl->Unlock(soundBuffer, buffer, bufferLen, buffer2, bufferLen2);

	result = notify->lpVtbl->SetNotificationPositions(notify, numbuffers, positionNotify);

	result = soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);
}
