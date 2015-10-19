#include "PointType.h"
//#include "VectorType.h"

VectorType PointType::getVectorFromPoint()
{
	//This is the vector from the origin to the input point.
	VectorType results;
	results.x = x;
	results.y = y;
	results.z = z;
	return results;
}

VectorType PointType::vectorFromHereToPoint(PointType secondPoint)
{
	//find the vector from THIS point, to the second point.
	VectorType results;
	results.x = secondPoint.x - x;
	results.y = secondPoint.y - y;
	results.z = secondPoint.z - z;
	return results;
}