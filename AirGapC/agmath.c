#include "agmath.h"

#include <math.h>


float ag_sin(float value)
{
	return sinf(value);
}

float ag_cos(float value)
{
	return cosf(value);
}

float ag_abs(float value)
{
	return (value < 0.0f) ? -value : value;
}

float ag_sqrt(float value)
{
	return sqrtf(value);
}

float ag_exp(float value)
{
	return expf(value);
}
