#ifndef _Common_
#define _Common_

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//#include "PictureData.h"
//#include "MaterialType.h"

#include "PointType.h"
#include "VectorType.h"
#include "RayType.h"
#include "SphereType.h"

using namespace std;

//class MaterialType;
class PictureData;
//class SphereType;
//class RayType;
class MaterialType;

class Common
{ 
	public:


	static string trim(string);
	static vector<string> tokenizeString(string);
	static string getInputFileName(int, char*[]);
	
	static bool thresholdEquals(double, double);
	static double intersectSphere(RayType ray, SphereType sphere);
	static string getColorValue(double colorValue);
	static double clamp(double input);
	static PointType getPointFromVector(VectorType);
	static VectorType getVectorFromPoint(PointType);
	static VectorType vectorFromHereToPoint(PointType firstPoint, PointType secondPoint);
};

#endif