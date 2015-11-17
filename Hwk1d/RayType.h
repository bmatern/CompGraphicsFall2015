#ifndef _RayType_
#define _RayType_

#include "VectorType.h"
#include "PointType.h"

//class PointType, VectorType;

class RayType
{
	public:
	//If the ray is inside an object, the object's index 
	//of refraction can get stored here.
	bool insideObject;
	double indexOfRefraction;

	PointType origin;
	VectorType direction;
};

#endif