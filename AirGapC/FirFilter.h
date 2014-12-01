#include "DataTypes.h"

void(*FirFilter_ReportData)(ComplexPackage);
void FirFilter_OnData(ComplexPackage);

void FirFilter_InitGaussian();
void FirFilter_InitLowPass();
