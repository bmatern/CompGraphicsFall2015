#ifndef _PointType_
#define _PointType_

#include "VectorType.h"
//#include "Common.h"

class VectorType;

class PointType
{
	public:
	double x,y,z;

	VectorType getVectorFromPoint();
	VectorType vectorFromHereToPoint(PointType);
};

#endif

