#ifndef _MaterialType_
#define _MaterialType_

//#include "Common.h"
#include "ColorType.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//class ColorType;
//class Common;

class MaterialType
{
	public:

	bool hasTexture;
	string textureFilename;
	vector<ColorType> texturePixelArray;
	ColorType color;

	//Osλ is the color of the specular highlight.
	ColorType specularHighlight;

	//constants for the ambient, diffuse, and specular highlights.
	//Should be 0 <= k <= 1
	double ka, kd, ks;

	//n controls the effective rate of falloff in the intensity
	//maybe 2 -> 100
	int n;

	void loadTexture();

};

#endif