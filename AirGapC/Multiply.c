#include "Multiply.h"
#include <stdlib.h>
#include <math.h>


#define M_PI       3.14159265358979323846

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);

const int sampleRate;

void Multiply_OnData(ComplexPackage data)
{
	float frequency = 20000.0f;
	int position = 0;

	ComplexPackage ret;
	ret.count = data.count;
	ret.data = (Complex *)malloc(ret.count * sizeof(Complex));

	ComplexPackage test;
	test.count = data.count;
	test.data = (Complex *)malloc(test.count * sizeof(Complex));
	

	//TODO: datenstrom mit einer sinus/cosinus welle muliplizieren

	for (int i = 0; i < data.count; i++)
	{
		//TODO: sin & cos bei <position> mit <frequency> ausrechnen
		position++;

		//Data for a test if multiplication is correct:
		/*
		data.data[i].i = cos(2 * M_PI * position / 44100 * (-2000.0f)); 
		data.data[i].q = sin(2 * M_PI * position / 44100 * (-2000.0f));

		test.data[i].i = cos(2 * M_PI * position / 44100 * 18000.0f);
		test.data[i].q = sin(2 * M_PI * position / 44100 * 18000.0f);
		*/

		//result when you multiply two complex numbers: x1*x2-y1*y2+j*(x1y2+x2y1) 
		//Source: http://www.physik-multimedial.de/cvpmm/sle/komplexzahl/prodquoimgb.html
		//Source: http://de.mathworks.com/help/dsp/ref/sinewave.html
		
		ret.data[i].i = data.data[i].i * cos(2 * M_PI * position / 44100 * frequency) - data.data[i].q * sin(2 * M_PI * position / 44100 * frequency);
		ret.data[i].q = data.data[i].i * sin(2 * M_PI * position / 44100 * frequency) + data.data[i].q * cos(2 * M_PI * position / 44100 * frequency);
		
		//TODO: berechnete Zahl mit Zahl aus <data[i]> multiplizieren
	}
	//TODO: alle berechneten Zahlen zurückgeben
	Multiply_ReportData(ret);
	free(ret.data);
	return;
}
