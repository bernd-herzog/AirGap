#include <jni.h>
#include "main.h"

#include "MemorySink.h"
#include "FileSink.h"
#include "BinarySlicer.h"
#include "QuadraturDemodulator.h"
#include "ClockRecovery.h"
#include "Multiply.h"
#include "FirFilter.h"
#include "Depacketizer.h"

#include <stdbool.h>
#include "agmath.h"

#include <stdio.h>
#include <stdlib.h>

/*extern "C"
{
	JNIEXPORT jstring JNICALL Java_de_nachregenkommtsonne_airgapclient_MainActivity_getMessage (JNIEnv * env, jobject jObj);
}*/
void InitReceiver();
int once = 0;

JNIEXPORT jstring JNICALL Java_de_nachregenkommtsonne_airgapclient_MainActivity_getMessage (JNIEnv * env, jobject jObj, jshortArray samples)
{
	int len = (*env)->GetArrayLength(env, samples);

	jshort *sampl = (*env)->GetShortArrayElements(env, samples, 0);

	if (once == 0){
		once = 1;
		InitReceiver();
	}

	ComplexPackage p;

	p.count = len;
	p.data = (Complex *) malloc(len * sizeof(Complex));


	for (int i = 0; i < len; i++){
		float sampleValue = ((float)sampl[i] / 0x8000);

		p.data[i].i = sampleValue;
		p.data[i].q = sampleValue;
	}

	Multiply_OnData(p);

	MemorySink_data[MemorySink_len] = 0;

	return (*env)->NewStringUTF(env, MemorySink_data);
}

void InitReceiver(){
	FirFilter_InitLowPass();
	Multiply_SetFrequency(-ag_BASE_FREQUENCY);
	ClockRecovery_Init();
	Depacketizer_Init();

	CONNECT(Multiply, FirFilter);
	CONNECT(FirFilter, QuadraturDemodulator);
	CONNECT(QuadraturDemodulator, ClockRecovery);
	CONNECT(ClockRecovery, BinarySlicer);
	CONNECT(BinarySlicer, Depacketizer);
	CONNECT(Depacketizer, MemorySink);

	MemorySink_len = 0;
	MemorySink_data = 0;

}
