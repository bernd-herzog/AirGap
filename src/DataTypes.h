#pragma once

#include <stdbool.h>

typedef struct {
//TODO: rename
	float i;
	float q;
} Complex;

typedef struct {
	short *data;
	int count;
} ShortPackage;

typedef struct {
	float *data;
	int count;
} FloatPackage;

typedef struct {
	Complex *data;
	int count;
} ComplexPackage;

typedef struct {
	bool *data;
	int count;
} BoolPackage;

