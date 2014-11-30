#include "QuadraturDemodulator.h"
#include "DataTypes.h"
#include <stdlib.h>

extern void(*QuadraturDemodulator_ReportData)(FloatPackage);
extern void QuadraturDemodulator_OnData(ComplexPackage);

float fast_atan2f(float, float);
float fabs(float);

void QuadraturDemodulator_OnData(ComplexPackage packet)
{
	FloatPackage ret;
	ret.count = packet.count;
	ret.data = (float *)malloc(packet.count * sizeof(float));

	for (int i = 1; i < packet.count; i++){

		Complex conjugate;
		conjugate.i = packet.data[i - 1].i;
		conjugate.q = -packet.data[i - 1].q;

		Complex in = packet.data[i];

		Complex product;
		product.i = in.i * conjugate.i - (in.q * conjugate.q);
		product.q = in.i * conjugate.q + (in.q * conjugate.i);
		
		float gain = 1.0f;

		float ret = fast_atan2f(product.i, product.q);
	}

	QuadraturDemodulator_ReportData(ret);

	free(ret.data);
}

#define REAL float

#define TAN_MAP_RES 0.003921569 /* (smallest non-zero value in table) */
#define RAD_PER_DEG 0.017453293
#define TAN_MAP_SIZE 256

REAL fast_atan_table[257] = {
	0.000000e+00f, 3.921549e-03f, 7.842976e-03f, 1.176416e-02f,
	1.568499e-02f, 1.960533e-02f, 2.352507e-02f, 2.744409e-02f,
	3.136226e-02f, 3.527947e-02f, 3.919560e-02f, 4.311053e-02f,
	4.702413e-02f, 5.093629e-02f, 5.484690e-02f, 5.875582e-02f,
	6.266295e-02f, 6.656816e-02f, 7.047134e-02f, 7.437238e-02f,
	7.827114e-02f, 8.216752e-02f, 8.606141e-02f, 8.995267e-02f,
	9.384121e-02f, 9.772691e-02f, 1.016096e-01f, 1.054893e-01f,
	1.093658e-01f, 1.132390e-01f, 1.171087e-01f, 1.209750e-01f,
	1.248376e-01f, 1.286965e-01f, 1.325515e-01f, 1.364026e-01f,
	1.402496e-01f, 1.440924e-01f, 1.479310e-01f, 1.517652e-01f,
	1.555948e-01f, 1.594199e-01f, 1.632403e-01f, 1.670559e-01f,
	1.708665e-01f, 1.746722e-01f, 1.784728e-01f, 1.822681e-01f,
	1.860582e-01f, 1.898428e-01f, 1.936220e-01f, 1.973956e-01f,
	2.011634e-01f, 2.049255e-01f, 2.086818e-01f, 2.124320e-01f,
	2.161762e-01f, 2.199143e-01f, 2.236461e-01f, 2.273716e-01f,
	2.310907e-01f, 2.348033e-01f, 2.385093e-01f, 2.422086e-01f,
	2.459012e-01f, 2.495869e-01f, 2.532658e-01f, 2.569376e-01f,
	2.606024e-01f, 2.642600e-01f, 2.679104e-01f, 2.715535e-01f,
	2.751892e-01f, 2.788175e-01f, 2.824383e-01f, 2.860514e-01f,
	2.896569e-01f, 2.932547e-01f, 2.968447e-01f, 3.004268e-01f,
	3.040009e-01f, 3.075671e-01f, 3.111252e-01f, 3.146752e-01f,
	3.182170e-01f, 3.217506e-01f, 3.252758e-01f, 3.287927e-01f,
	3.323012e-01f, 3.358012e-01f, 3.392926e-01f, 3.427755e-01f,
	3.462497e-01f, 3.497153e-01f, 3.531721e-01f, 3.566201e-01f,
	3.600593e-01f, 3.634896e-01f, 3.669110e-01f, 3.703234e-01f,
	3.737268e-01f, 3.771211e-01f, 3.805064e-01f, 3.838825e-01f,
	3.872494e-01f, 3.906070e-01f, 3.939555e-01f, 3.972946e-01f,
	4.006244e-01f, 4.039448e-01f, 4.072558e-01f, 4.105574e-01f,
	4.138496e-01f, 4.171322e-01f, 4.204054e-01f, 4.236689e-01f,
	4.269229e-01f, 4.301673e-01f, 4.334021e-01f, 4.366272e-01f,
	4.398426e-01f, 4.430483e-01f, 4.462443e-01f, 4.494306e-01f,
	4.526070e-01f, 4.557738e-01f, 4.589307e-01f, 4.620778e-01f,
	4.652150e-01f, 4.683424e-01f, 4.714600e-01f, 4.745676e-01f,
	4.776654e-01f, 4.807532e-01f, 4.838312e-01f, 4.868992e-01f,
	4.899573e-01f, 4.930055e-01f, 4.960437e-01f, 4.990719e-01f,
	5.020902e-01f, 5.050985e-01f, 5.080968e-01f, 5.110852e-01f,
	5.140636e-01f, 5.170320e-01f, 5.199904e-01f, 5.229388e-01f,
	5.258772e-01f, 5.288056e-01f, 5.317241e-01f, 5.346325e-01f,
	5.375310e-01f, 5.404195e-01f, 5.432980e-01f, 5.461666e-01f,
	5.490251e-01f, 5.518738e-01f, 5.547124e-01f, 5.575411e-01f,
	5.603599e-01f, 5.631687e-01f, 5.659676e-01f, 5.687566e-01f,
	5.715357e-01f, 5.743048e-01f, 5.770641e-01f, 5.798135e-01f,
	5.825531e-01f, 5.852828e-01f, 5.880026e-01f, 5.907126e-01f,
	5.934128e-01f, 5.961032e-01f, 5.987839e-01f, 6.014547e-01f,
	6.041158e-01f, 6.067672e-01f, 6.094088e-01f, 6.120407e-01f,
	6.146630e-01f, 6.172755e-01f, 6.198784e-01f, 6.224717e-01f,
	6.250554e-01f, 6.276294e-01f, 6.301939e-01f, 6.327488e-01f,
	6.352942e-01f, 6.378301e-01f, 6.403565e-01f, 6.428734e-01f,
	6.453808e-01f, 6.478788e-01f, 6.503674e-01f, 6.528466e-01f,
	6.553165e-01f, 6.577770e-01f, 6.602282e-01f, 6.626701e-01f,
	6.651027e-01f, 6.675261e-01f, 6.699402e-01f, 6.723452e-01f,
	6.747409e-01f, 6.771276e-01f, 6.795051e-01f, 6.818735e-01f,
	6.842328e-01f, 6.865831e-01f, 6.889244e-01f, 6.912567e-01f,
	6.935800e-01f, 6.958943e-01f, 6.981998e-01f, 7.004964e-01f,
	7.027841e-01f, 7.050630e-01f, 7.073330e-01f, 7.095943e-01f,
	7.118469e-01f, 7.140907e-01f, 7.163258e-01f, 7.185523e-01f,
	7.207701e-01f, 7.229794e-01f, 7.251800e-01f, 7.273721e-01f,
	7.295557e-01f, 7.317307e-01f, 7.338974e-01f, 7.360555e-01f,
	7.382053e-01f, 7.403467e-01f, 7.424797e-01f, 7.446045e-01f,
	7.467209e-01f, 7.488291e-01f, 7.509291e-01f, 7.530208e-01f,
	7.551044e-01f, 7.571798e-01f, 7.592472e-01f, 7.613064e-01f,
	7.633576e-01f, 7.654008e-01f, 7.674360e-01f, 7.694633e-01f,
	7.714826e-01f, 7.734940e-01f, 7.754975e-01f, 7.774932e-01f,
	7.794811e-01f, 7.814612e-01f, 7.834335e-01f, 7.853983e-01f,
	7.853983e-01f
};

float fabs(float val)
{
	if (val < val)
		return -val;
	return val;
}

float fast_atan2f(float x, float y)
{
	REAL x_abs, y_abs, z;
	REAL alpha, angle, base_angle;
	int index;

	/* don't divide by zero! */		// FIXME could get hosed with -0.0
	if ((y == 0.0) && (x == 0.0))
		return 0.0;

	/* normalize to +/- 45 degree range */
	y_abs = fabs(y);
	x_abs = fabs(x);
	//z = (y_abs < x_abs ? y_abs / x_abs : x_abs / y_abs);
	if (y_abs < x_abs)
		z = y_abs / x_abs;
	else
		z = x_abs / y_abs;

	/* when ratio approaches the table resolution, the angle is */
	/* best approximated with the argument itself... */
	if (z < TAN_MAP_RES)
		base_angle = z;
	else {
		/* find index and interpolation value */
		alpha = z * (REAL)TAN_MAP_SIZE - .5f;
		index = (int)alpha;
		alpha -= (REAL)index;
		/* determine base angle based on quadrant and */
		/* add or subtract table value from base angle based on quadrant */
		base_angle = fast_atan_table[index];
		base_angle +=
			(fast_atan_table[index + 1] - fast_atan_table[index]) * alpha;
	}

	if (x_abs > y_abs) { /* -45 -> 45 or 135 -> 225 */
		if (x >= 0.0) { /* -45 -> 45 */
			if (y >= 0.0)
				angle = base_angle; /* 0 -> 45, angle OK */
			else
				angle = -base_angle; /* -45 -> 0, angle = -angle */
		}
		else { /* 135 -> 180 or 180 -> -135 */
			angle = 3.14159265358979323846f;
			if (y >= 0.0)
				angle -= base_angle; /* 135 -> 180, angle = 180 - angle */
			else
				angle = base_angle - angle; /* 180 -> -135, angle = angle - 180 */
		}
	}
	else { /* 45 -> 135 or -135 -> -45 */
		if (y >= 0.0) { /* 45 -> 135 */
			angle = 1.57079632679489661923f;
			if (x >= 0.0)
				angle -= base_angle; /* 45 -> 90, angle = 90 - angle */
			else
				angle += base_angle; /* 90 -> 135, angle = 90 + angle */
		}
		else { /* -135 -> -45 */
			angle = -1.57079632679489661923f;
			if (x >= 0.0)
				angle += base_angle; /* -90 -> -45, angle = -90 + angle */
			else
				angle -= base_angle; /* -135 -> -90, angle = -90 - angle */
		}
	}

#ifdef ZERO_TO_TWOPI
	if (angle < 0)
		return (angle + TWOPI);
	else
		return (angle);
#else
	return (angle);
#endif
}

