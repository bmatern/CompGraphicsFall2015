#ifndef _MaterialType_
#define _MaterialType_

#include "ColorType.h"

class MaterialType
{
	public:

	ColorType color;

	//Osλ is the color of the specular highlight.
	ColorType specularHighlight;

	//constants for the ambient, diffuse, and specular highlights.
	//Should be 0 <= k <= 1
	double ka, kd, ks;

	//n controls the effective rate of falloff in the intensity
	//maybe 2 -> 100
	int n;

};

#endif