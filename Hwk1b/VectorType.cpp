#include "VectorType.h"
#include <math.h>

PointType VectorType::getPointFromVector()
{
	//This is the point at the tip of a vector, if assumed to originate at 0,0,0
	PointType results;
	results.x = x;
	results.y = y;
	results.z = z;
	return results;
}

double VectorType::dotProduct(VectorType secondVector)
{
	//Calculate dot product of 2 vectors
	//s = x1 x2 + y1 y2 + z1 z2
	double results = 
		x * secondVector.x +
		y * secondVector.y +
		z * secondVector.z;
	return results;
}

VectorType VectorType::crossProduct(VectorType secondVector)
{
	//calculate cross product firstVector X secondVector
	//u x v = 
		// (uy * vz - uz * vy 
		//, uz * vx - ux * vz 
		//, ux * vy - uy * vx)
	VectorType results;
	results.x = (y * secondVector.z) - (z * secondVector.y);
	results.y = (z * secondVector.x) - (x * secondVector.z);
	results.z = (x * secondVector.y) - (y * secondVector.x);
	return results;
}

double VectorType::vectorLength()
{
	//calculate the length of a vector
	//sqrt(x^2 + y^2 + z^2)
	return sqrt(x * x
		+ y * y
		+ z * z);
}

VectorType VectorType::normalizeVector()
{
	//normalize the vector to length 1.
	double normalizeScalar = 1.0 / vectorLength();
	VectorType results = multiplyVector(normalizeScalar);
	return results;
}

VectorType VectorType::multiplyVector(double scalar)
{
	//Multiply a vector by a scalar
	VectorType results;	
	results.x = x * scalar;
	results.y = y * scalar;
	results.z = z * scalar;	
	return results;
}

VectorType VectorType::addVectors(VectorType secondVector)
{
	//respectively add the components of the two vectors.
	VectorType results;
	results.x = x + secondVector.x;
	results.y = y + secondVector.y;
	results.z = z + secondVector.z;
	return results;
}

VectorType VectorType::addVectors(PointType originPoint)
{
	//If we need to add a vector to a point.
	return addVectors(originPoint.getVectorFromPoint());
}
