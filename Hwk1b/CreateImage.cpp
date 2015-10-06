/*
 * Ben Matern
 * Homework 1b
 * Light and Shadow
 * Computer Graphics
 *
 * This program can create a ASCII Format image from an input file.
 * Main method is at the very bottom.
 *
 */

#include "Common.h"
#include "PictureData.h"
using namespace std;



int main( int argc, char *argv[] )
{
	cout << "Creating a new image.\n";

	PictureData pictureConfig;

//Read commandline args to get the input file name.
	pictureConfig.inputFileName = Common::getInputFileName(argc, argv);
	cout << "Input:" << pictureConfig.inputFileName << ":\n";

//Read input file to get the scene information.
	try
	{
		pictureConfig.loadSceneInformation();
	}
	catch(int e)
	{
		cout << "Exception occured during loadSceneInformation.  Doublecheck your input file.  Cannot recover." << endl;
		return(0);
	}

//Perform necessary preliminary calculations.
//Set up the viewing window etc.
	cout << "Starting Preliminary Calculations" << endl;
	pictureConfig.setViewingWindow();

//foreach pixel in image array, trace a ray
	pictureConfig.traceRays();
	
//Write the data to a image file.
	Common::writeImageFile(pictureConfig);

	return 0;
}


