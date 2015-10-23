#ifndef _LightType_
#define _LightType_

#include "PointType.h"
#include "VectorType.h"
#include "ColorType.h"

//class PointType;
//class VectorType;
//class ColorType;

class LightType
{
	public:
	//if idDirectional is true(1) the light source is directional.
	//Otherwise, it is a point light source.
	//it should really only have either a center or a direction.
	bool isDirectional;

	PointType center;
	VectorType direction;

	ColorType intensity;
};

#endif