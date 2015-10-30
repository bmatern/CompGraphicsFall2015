#ifndef _Demos_
#define _Demos_

#include <string>
#include <vector>
#include "PictureData.h"
using namespace std;

class Demos
{
	public:

	static void generateTextures();
	static void writeTexture(TextureType inputTexture);
	static PictureData stitchImages(vector <vector <ColorType> > images, int width, int height);
	static void shiftCameraDemo();
	static void shiftLightSource();
	static void shiftUpVector();
	static void shiftFOV();
	static void adjustSurface();

};

#endif