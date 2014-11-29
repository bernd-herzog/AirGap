#include "AudioSink.h"
#include "DataTypes.h"

#include <stdbool.h>
#include <stdio.h>
#include <dsound.h>
#include <math.h>

#define BUFFER_SIZE (1024*128)

extern void AudioSink_OnData(ComplexPackage);

void AudioSink_OnData(ComplexPackage data)
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

	LPDIRECTSOUNDBUFFER soundBuffer;
	result = directSound->lpVtbl->CreateSoundBuffer(directSound, &bufferDescription, &soundBuffer, NULL);

	LPVOID buffer=0;
	DWORD bufferLen=0;
	LPVOID buffer2=0;
	DWORD bufferLen2=0;

	result = soundBuffer->lpVtbl->Lock(soundBuffer, 0, BUFFER_SIZE, &buffer, &bufferLen, &buffer2, &bufferLen2, 0);
	
	short *samples = (short *)buffer;

	for (int i = 0; i < BUFFER_SIZE / 2; i++)
	{
		float x = (float)i;
		x *= 2 * 3.1415f;
		x /= 22;
		float y = sinf(x);

		samples[i] = (short) (y * 30000);
	}
	
	result = soundBuffer->lpVtbl->Unlock(soundBuffer, buffer, bufferLen, buffer2, bufferLen2);


	result = soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);

	Sleep(15000);


	//do something with data
}
