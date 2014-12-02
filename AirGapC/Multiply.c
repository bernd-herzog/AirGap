#include "Multiply.h"
#include <stdlib.h>

extern void(*Multiply_ReportData)(ComplexPackage);
extern void Multiply_OnData(ComplexPackage);

const int sampleRate;

void Multiply_OnData(ComplexPackage data)
{
	float frequency = 20000.0f;
	int position = 0;

	//TODO: datenstrom mit einer sinus/cosinus welle muliplizieren

	for (int i = 0; i < data.count; i++)
	{
		//TODO: sin & cos bei <position> mit <frequency> ausrechnen
		position++;
		//TODO: berechnete Zahl mit Zahl aus <data[i]> multiplizieren
	}
	//TODO: alle berechneten Zahlen zurückgeben

	return;
}
