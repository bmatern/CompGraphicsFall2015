#ifndef _FaceType_
#define _FaceType_

#include "PointType.h"
#include "MaterialType.h"
#include "VectorType.h"
#include "TextureCoordinateType.h"

//class MaterialType;
//class PointType;

class FaceType
{
	public:
	//int v1, v2, v3;
	PointType v1,v2,v3;
	TextureCoordinateType vt1, vt2, vt3;
	VectorType vn1, vn2, vn3;
	VectorType e1, e2;
	double A,B,C,D;
	//double radius;
	MaterialType material;
	//materialIndex;
};

#endif