#include "FirFilter.h"
#include "agmath.h"
#include <stdlib.h>

extern void(*FirFilter_ReportData)(ComplexPackage);
extern void FirFilter_OnData(ComplexPackage);

extern void FirFilter_InitGaussian();
extern void FirFilter_InitLowPass();
void Faltung(Complex*, const Complex*, const Complex*, int);

Complex *_taps;
int _numTaps;
Complex *_buffer;
int _bufferPosition = 0;

void FirFilter_OnData(ComplexPackage packet)
{
	ComplexPackage ret;
	ret.count = packet.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));

	for (int i = 0; i < packet.count; i++)
	{
		if (_bufferPosition != 0)
			_buffer[_bufferPosition - 1] = packet.data[i];
		_buffer[_bufferPosition - 1 + _numTaps] = packet.data[i];
		
		Faltung(&ret.data[i], _buffer + _bufferPosition, _taps, _numTaps);

		ret.data[i].i /= ag_SAMPLES_PER_SYMBOL;
		ret.data[i].q /= ag_SAMPLES_PER_SYMBOL;

		_bufferPosition = (_bufferPosition + 1) % _numTaps;
		if (_bufferPosition >= _numTaps)
			_bufferPosition -= _numTaps;
	}

	FirFilter_ReportData(ret);
	free(ret.data);
}

void FirFilter_InitGaussian()
{
	float spb = ag_SAMPLES_PER_SYMBOL;
	float bt = 0.65f; // Gaussian filter bandwidth * symbol time.
	int ntapsGaussian = 2 * ag_SAMPLES_PER_SYMBOL;

	float *tapsGaussian = (float *)calloc(ntapsGaussian, sizeof(float));
	float gain = 1.0f;
	
	float scale = 0.0f;
	float dt = 1.0f / spb;
	float s = 1.0f / (ag_sqrt(ag_log(2.0f)) / (2 * ag_PI*bt));
	float t0 = -0.5f * ntapsGaussian;

	for (int i = 0; i < ntapsGaussian; i++) {
		t0++;
		float ts = s*dt*t0;
		tapsGaussian[i] = ag_exp(-0.5f *ts*ts);
		scale += tapsGaussian[i];
	}

	for (int i = 0; i < ntapsGaussian; i++)
		tapsGaussian[i] = tapsGaussian[i] / scale * gain;

	//convolve with rectangular window size: ag_SAMPLES_PER_SYMBOL
	int ntaps = ntapsGaussian + ag_SAMPLES_PER_SYMBOL -1;
	Complex *taps = (Complex *)calloc(ntaps, sizeof(Complex));

	for (int n = 0; n < ntaps; n++)
	{
		size_t kmin, kmax, k;

		taps[n].i = 0;

		kmin = (n >= ntapsGaussian - 1) ? n - (ntapsGaussian - 1) : 0;
		{
			kmax = (n < spb - 1)
				? n 
				: ag_SAMPLES_PER_SYMBOL - 1;
		}

		for (k = kmin; k <= kmax; k++)
		{
			taps[n].i += 1.0f * tapsGaussian[n - k];
		}
	}

	_taps = taps;
	_numTaps = ntaps;
	_buffer = (Complex *)calloc((_numTaps * 2 - 1), sizeof(Complex));
}

void FirFilter_InitLowPass()
{
	// variables
	float maxAttenuation = 53; //for hamming
	float transitionWidth = 100.0f;
	float cutoffFrequency = 1000.0f;
	float gain = 1.0f;

	// calculate num taps for hamming WINDOW
	int ntapsHammingWindow = (int)(maxAttenuation*ag_SAMPLERATE / (22.0*transitionWidth));
	if ((ntapsHammingWindow & 1) == 0)
		ntapsHammingWindow++;

	// calculate hamming window
	float *tapsHammingWindow = (float *)calloc(ntapsHammingWindow, sizeof(float));
	float M2 = (float)ntapsHammingWindow - 1;
	for (int n = 0; n < ntapsHammingWindow; n++)
		tapsHammingWindow[n] = 0.54f - 0.46f * ag_cos((2.f * ag_PI * n) / M2);

	//calculate LowPass Window
	int ntaps = ntapsHammingWindow;
	Complex *taps = (Complex *)calloc(ntaps, sizeof(Complex));

	int M = (ntaps - 1) / 2;
	float fwT0 = 2.f * ag_PI * cutoffFrequency / ag_SAMPLERATE;
	for (int n = -M; n <= M; n++) {
		if (n == 0)
			taps[n + M].i = fwT0 / ag_PI * tapsHammingWindow[n + M];
		else {
			taps[n + M].i = ag_sin(n * fwT0) / (n * ag_PI) * tapsHammingWindow[n + M];
		}
	}

	float fmax = taps[0 + M].i;
	for (int n = 1; n <= M; n++)
		fmax += 2 * taps[n + M].i;
	gain /= fmax;
	for (int i = 0; i < ntaps; i++)
		taps[i].i *= gain;

	//have actual Taps now!!!
	_taps = taps;
	_numTaps = ntaps;
	_buffer = (Complex *)calloc((_numTaps * 2 - 1), sizeof(Complex));
}


void Faltung(Complex* result, const Complex* input, const Complex* taps, int num_points)
{
	Complex * in = (Complex*)input;
	Complex * tp = (Complex*)taps;

	Complex sum0 = { 0.f, 0.f };

	for (int i = 0; i < num_points; ++i) {
		
		sum0.i += in[0].i * tp[0].i - in[0].q * tp[0].q; // real
		sum0.q += in[0].i * tp[0].q + in[0].q * tp[0].i; // img

		in ++;
		tp ++;
	}

	result->i = sum0.i;
	result->q = sum0.q;
}
