#ifndef _Common_
#define _Common_

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "RayType.h"
#include "SphereType.h"
#include "PictureData.h"

using namespace std;

class PictureData;

class Common
{ 
	public:

	static void createImage(string inputFileName);
	static string trim(string);
	static vector<string> tokenizeString(string);
	static string getInputFileName(int, char*[]);
	static void writeImageFile(PictureData data);
	static bool thresholdEquals(double, double);
	static double intersectSphere(RayType ray, SphereType sphere);
	static string getColorValue(double colorValue);
	static double clamp(double input);
};

#endif