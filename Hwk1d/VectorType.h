#ifndef _VectorType_
#define _VectorType_

#include <math.h>

class VectorType
{
	public:
	double x,y,z;

	double dotProduct(VectorType);
	VectorType crossProduct(VectorType);
	double vectorLength();
	VectorType normalizeVector();
	VectorType multiplyVector(double scalar);
	VectorType addVectors(VectorType);
};

#endif