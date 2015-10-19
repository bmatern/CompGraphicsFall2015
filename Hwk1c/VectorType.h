#ifndef _VectorType_
#define _VectorType_

#include <math.h>
#include "PointType.h"

//forward declaration
class PointType;

class VectorType
{
	public:
	double x,y,z;

	PointType getPointFromVector();
	double dotProduct(VectorType);
	VectorType crossProduct(VectorType);
	double vectorLength();
	VectorType normalizeVector();
	VectorType multiplyVector(double scalar);
	VectorType addVectors(VectorType);
	VectorType addVectors(PointType);

};

#endif