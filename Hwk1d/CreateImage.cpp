/*
 * Ben Matern
 * Homework 1b
 * Light and Shadow
 * Computer Graphics
 *
 * This program can create a ASCII Format image from an input file.
 *
 */

#include "Common.h"
#include "Demos.h"
using namespace std;

void runDemos()
{
	//Demos::shiftCameraDemo();
	//Demos::shiftLightSource();
	//Demos::shiftUpVector();
	//Demos::shiftFOV();
	//Demos::adjustSurface();
	Demos::generateTextures();
}

int main( int argc, char *argv[] )
{
	


	cout << "Creating a new image.\n";

//Read commandline args to get the input file name.
	string inputFileName = Common::getInputFileName(argc, argv);
	if(inputFileName.length() > 0)
	{
		PictureData newPicture;
		newPicture.createImage(inputFileName);
	}
	else
	{
		runDemos();
	}


	

	return 0;
}


