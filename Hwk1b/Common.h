#ifndef _Common_
#define _Common_

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ColorType.h"

using namespace std;

class Common
{
	public:

	static string trim(string);
	static vector<string> tokenizeString(string);
	static string getInputFileName(int, char*[]);
	static void writeImageFile(ColorType[], int, int, string);
	static bool thresholdCompare(double, double);
};

#endif