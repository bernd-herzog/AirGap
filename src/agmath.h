#include <time.h>

float ag_sin(float);
float ag_cos(float);

float ag_abs(float);

float ag_sqrt(float);
float ag_exp(float);
float ag_log(float);

#define ag_PI (3.14159265358979323846f)

#ifdef ANDROID
	#define START_TIMER
	#define PRINT_TIMER(a)

	#define ag_SAMPLERATE (44100)
	#define ag_SAMPLES_PER_SYMBOL (147)
#else
	#define START_TIMER clock_t rawtime; rawtime=clock()
	#define PRINT_TIMER(a) clock_t rawtime2; rawtime2=clock(); printf("%s took: %d\n", a, rawtime2 - rawtime)

	#define ag_SAMPLERATE (48000)
	#define ag_SAMPLES_PER_SYMBOL (160)
#endif

#define ag_FREQUENCY_SHIFT (50.f)
#define ag_FREQUENCY_SHIFT_SYMBOLS (8)

#define ag_BASE_FREQUENCY (19000.f)

#define ag_PACKETSIZE (8)
#define ag_ERRORCORRECTIONSIZE (1)
#define ag_PREAMBLESIZE (2)
