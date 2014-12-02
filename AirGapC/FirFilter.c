#include "FirFilter.h"
#include "agmath.h"

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
	int ntaps = 0;
	//TODO: speicher reservieren
	float *taps;
	float gain = 1.0f;

	//TODO: was machen diese parameter und welche werte brauchensie?
	float spb = 1.0f;
	float bt = 1.0f;

	
	float scale = 0;
	float dt = 1.0 / spb;
	float s = 1.0 / (ag_sqrt(log(2.0)) / (2 * ag_PI*bt));
	float t0 = -0.5 * ntaps;
	float ts;

	for (int i = 0; i<ntaps; i++) {
		t0++;
		ts = s*dt*t0;
		taps[i] = ag_exp(-0.5*ts*ts);
		scale += taps[i];
	}
	for (int i = 0; i<ntaps; i++)
		taps[i] = taps[i] / scale * gain;

}

void FirFilter_InitLowPass()
{
	float transition_width = 100.0f;
	

	//TODO: welcher wert ist hier sinnvoll?
	float attenuation_dB = 1.0f;

	//hamming
	int ntaps = (int)(attenuation_dB*ag_SAMPLERATE / (22.0*transition_width));
	if ((ntaps & 1) == 0)        // if even...
		ntaps++;                // ...make odd


	//TODO: speicher reservieren
	float *hammingTaps;
	float hammingNTaps = 0;

	float u = hammingNTaps - 1;

	for (int n = 0; n < ntaps; n++)
		taps[n] = 0.54 - 0.46 * cos((2 * ag_PI * n) / u);


	// actual taps

	float cutoff_freq = 1000.0f;
	//sanity_check_1f(ag_SAMPLERATE, cutoff_freq, transition_width);

	

	// construct the truncated ideal impulse response
	// [sin(x)/x for the low pass case]

	float *taps;
	float *w = hammingTaps;

	int M = (ntaps - 1) / 2;
	double fwT0 = 2 * ag_PI * cutoff_freq / ag_SAMPLERATE;

	for (int n = -M; n <= M; n++)
	{
		int z = n + M;
		if (n == 0)
		{
			taps[z] = fwT0 / ag_PI * w[z];
		}
		else
		{
			// a little algebra gets this into the more familiar sin(x)/x form
			taps[z] = sin(n * fwT0) / (n * ag_PI) * w[z];
		}
	}

	// find the factor to normalize the gain, fmax.
	// For low-pass, gain @ zero freq = 1.0


	double fmax = taps[(ntaps - 1) / 2];

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
