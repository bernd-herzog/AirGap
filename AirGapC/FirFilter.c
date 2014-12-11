#include "FirFilter.h"
#include "agmath.h"
#include <stdlib.h>

extern void(*FirFilter_ReportData)(ComplexPackage);
extern void FirFilter_OnData(ComplexPackage);

extern void FirFilter_InitGaussian();
extern void FirFilter_InitLowPass();


float *taps;
int numTaps;

void FirFilter_OnData(ComplexPackage packet)
{



	
	FirFilter_ReportData(packet);
}

void FirFilter_InitGaussian()
{

	//TODO: was machen diese parameter und welche werte brauchensie?
	float spb = 1.0f;
	float bt = 1.0f;
	int ntaps = 0;

	float *taps = (float *) malloc (ntaps * sizeof(float));
	float gain = 1.0f;
	
	float scale = 0.0f;
	float dt = 1.0f / spb;
	float s = 1.0f / (ag_sqrt(ag_log(2.0f)) / (2 * ag_PI*bt));
	float t0 = -0.5f * ntaps;
	float ts;

	for (int i = 0; i<ntaps; i++) {
		t0++;
		ts = s*dt*t0;
		taps[i] = ag_exp(-0.5f *ts*ts);
		scale += taps[i];
	}
	for (int i = 0; i<ntaps; i++)
		taps[i] = taps[i] / scale * gain;

}

void FirFilter_InitLowPass()
{
	float transition_width = 100.0f;
	float cutoff_freq = 1000.0f;

	//TODO: welcher wert ist hier sinnvoll?
	float attenuation_dB = 1.0f;

	//hamming window
	int ntaps = (int)(attenuation_dB*ag_SAMPLERATE / (22.0*transition_width));;
	if ((ntaps & 1) == 0)        // if even...
		ntaps++;                // ...make odd

	float *hammingTaps = (float *)malloc(ntaps * sizeof(float));

	float u = (float)(ntaps - 1);

	for (int n = 0; n < ntaps; n++)
		hammingTaps[n] = 0.54f - 0.46f * ag_cos((2 * ag_PI * n) / u);

	// actual taps
	float *taps = (float *) malloc (ntaps * sizeof(float));
	float *w = hammingTaps;

	int M = (ntaps - 1) / 2;
	float fwT0 = 2 * ag_PI * cutoff_freq / ag_SAMPLERATE;

	for (int n = -M; n <= M; n++)
	{
		int z = n + M;
		if (n == 0)
		{
			taps[z] = fwT0 / ag_PI * w[z];
		}
		else
		{
			taps[z] = ag_sin(n * fwT0) / (n * ag_PI) * w[z];
		}
	}

	float fmax = taps[(ntaps - 1) / 2];

	for (int n = 1; n <= M; n++)
	{
		int z = n + M;
		fmax += 2 * taps[z];
	}

	float gain = 1.0f;
	gain /= fmax;    // normalize

	for (int i = 0; i < ntaps; i++)
		taps[i] *= gain;
}
