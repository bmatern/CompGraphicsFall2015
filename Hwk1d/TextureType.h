#ifndef _TextureType_
#define _TextureType_

//#include "VectorType.h"
//#include "PointType.h"

#include "ColorType.h"
#include <vector>
#include <string>

//class PointType, VectorType;

using namespace std;

class TextureType
{
	public:
	int dimension;
	//dimension is height and width.  I will always expect square textures.
	vector<ColorType> pixelArray;
	string textureFilename;
	//PointType origin;
	//VectorType direction;
};

#endif