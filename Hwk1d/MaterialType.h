#ifndef _MaterialType_
#define _MaterialType_

//#include "Common.h"
#include "ColorType.h"
//#include "TextureType.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

//class ColorType;
//class Common;

class MaterialType
{
	public:

	//bool hasTexture;
	int textureIndex;
	//TextureType texture;
	ColorType color;

	//Osλ is the color of the specular highlight.
	ColorType specularHighlight;

	//constants for the ambient, diffuse, and specular highlights.
	//Should be 0 <= k <= 1
	double ka, kd, ks;

	//n controls the effective rate of falloff in the intensity
	//maybe 2 -> 100
	int n;

	double opacity;

	//A material's fresnal reflectance is determined by the index of refraction of 
	//the two surfaces.
	//Air is 1.  Water is 1.333
	//Diamond is 2.4
	double indexOfRefraction;


};

#endif