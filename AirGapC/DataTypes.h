#pragma once

#include <stdbool.h>

typedef struct {
	float i;
	float q;
} Complex;

typedef struct {
	short *data;
	unsigned int count;
} ShortPackage;

typedef struct {
	float *data;
	unsigned int count;
} FloatPackage;

typedef struct {
	Complex *data;
	unsigned int count;
} ComplexPackage;

typedef struct {
	bool *data;
	unsigned int count;
} BoolPackage;

