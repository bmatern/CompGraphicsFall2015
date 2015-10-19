#ifndef _SphereType_
#define _SphereType_

#include "PointType.h"
#include "MaterialType.h"

class MaterialType;

class SphereType
{
	public:
	PointType center;
	double radius;
	MaterialType material;
};

#endif