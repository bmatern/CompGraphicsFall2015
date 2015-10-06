#ifndef _PictureData_
#define _PictureData_

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include "PointType.h"
#include "VectorType.h"
#include "MaterialType.h"
#include "ViewingWindowType.h"
#include "LightType.h"
#include "RayType.h"
#include "SphereType.h"
#include "Common.h"

using namespace std;


class PictureData
{
	public:

	//This width and height are in pixels.
	int width, height;
	
	//d, w, h are in world coordinates
	//d = distance from eye to window.  w=width window, h = height window.
	double d, w, h;
	
	RayType eyeRay;	
	
	VectorType upVector;
	VectorType u;
	VectorType v;	
	
	ColorType bgColor;
	MaterialType currentMaterial;
	
	vector<SphereType> spheres;

	vector<LightType> lights;
	
	//The angle of the horizontal field of view.  Theta.
	double fovH;
	
	//contains 4 points for the coordinates of the viewing window.
	ViewingWindowType viewingWindow;

	//deltaH and deltaV are VECTORS.  they indicate how far to increment the viewing window for each pixel.
	//deltaH = (ur - ul)/(pixwidth - 1)
	VectorType deltaH;
	VectorType deltaV;

	string inputFileName;

	//an array of colors for the output pixels.
	ColorType* pixelArray;

	//Method to set the viewing window.
	void loadSceneInformation();
	void setViewingWindow();
	ColorType traceRay(int x, int y);
	ColorType shadeRay(int x, int y, SphereType sphere);
	bool isShaded(int x, int y);
	void traceRays();
};

#endif