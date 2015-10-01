#ifndef _PointType_
#define _PointType_

#include "VectorType.h"

//forward declaration, am i doing this right?  
class VectorType;

class PointType
{
	public:
	double x,y,z;

	VectorType getVectorFromPoint();
	VectorType vectorFromHereToPoint(PointType);
};

#endif

