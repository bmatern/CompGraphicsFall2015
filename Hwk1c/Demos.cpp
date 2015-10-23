#include "Demos.h"

void Demos::shiftFOV()
{
	PictureData pictureConfig;

	pictureConfig.inputFileName = "bens_image.txt";
	//cout << "Input:" << pictureConfig.inputFileName << ":\n";

	pictureConfig.loadSceneInformation();

	vector <vector <ColorType> > images = vector <vector <ColorType> > (9);
	
	
	
	for(int i = 0; i < 9; i++)
	{
		pictureConfig.fovH = 20 + i * 17;
		pictureConfig.setViewingWindow();
		pictureConfig.traceRays();
		images[i] = (pictureConfig.pixelArray);
		
	}



	PictureData stitchedImage = stitchImages(images, pictureConfig.width, pictureConfig.height);
	stitchedImage.inputFileName = "ShiftFOVDemo.txt";
	//stitchedImages.outputFileName = "ShiftCameraDemo.ppm";
	
//Write the data to a image file.
	stitchedImage.writeImageFile();

}

void Demos::adjustSurface()
{
	PictureData pictureConfig;

	pictureConfig.inputFileName = "bens_image.txt";
	//cout << "Input:" << pictureConfig.inputFileName << ":\n";

	pictureConfig.loadSceneInformation();

	vector <vector <ColorType> > images = vector <vector <ColorType> > (9);
	
	
	
	for(int i = 0; i < 9; i++)
	{
		
		pictureConfig.setViewingWindow();
		pictureConfig.traceRays();
		images[i] = (pictureConfig.pixelArray);

		pictureConfig.spheres[1].material.ka = pictureConfig.spheres[1].material.ka * .8;
		pictureConfig.spheres[2].material.ka = pictureConfig.spheres[2].material.ka * .8;
		pictureConfig.spheres[3].material.kd = pictureConfig.spheres[1].material.kd * .8;
		pictureConfig.spheres[4].material.kd = pictureConfig.spheres[4].material.kd * .8;
		pictureConfig.spheres[5].material.ks = pictureConfig.spheres[5].material.ks * .8;
		pictureConfig.spheres[6].material.ks = pictureConfig.spheres[6].material.ks * .8;
		
	}



	PictureData stitchedImage = stitchImages(images, pictureConfig.width, pictureConfig.height);
	stitchedImage.inputFileName = "AdjustSurfaceDemo.txt";
	//stitchedImages.outputFileName = "ShiftCameraDemo.ppm";
	
//Write the data to a image file.
	stitchedImage.writeImageFile();

}

void Demos::shiftUpVector()
{
	PictureData pictureConfig;

	pictureConfig.inputFileName = "bens_image.txt";
	//cout << "Input:" << pictureConfig.inputFileName << ":\n";

	pictureConfig.loadSceneInformation();

	vector <vector <ColorType> > images = vector <vector <ColorType> > (9);
	
	pictureConfig.upVector.x = -4;
	
	for(int i = 0; i < 9; i++)
	{
		pictureConfig.upVector.x = -4 + i;
		pictureConfig.setViewingWindow();
		pictureConfig.traceRays();
		images[i] = (pictureConfig.pixelArray);
		
	}


	PictureData stitchedImage = stitchImages(images, pictureConfig.width, pictureConfig.height);
	stitchedImage.inputFileName = "ShiftUpVectorDemo.txt";
	//stitchedImages.outputFileName = "ShiftCameraDemo.ppm";
	
//Write the data to a image file.
	stitchedImage.writeImageFile();
}

void Demos::shiftLightSource()
{
	PictureData pictureConfig;

	pictureConfig.inputFileName = "bens_image.txt";
	//cout << "Input:" << pictureConfig.inputFileName << ":\n";

//Read input file to get the scene information.
	try
	{
		pictureConfig.loadSceneInformation();
	}
	catch(int e)
	{
		cout << "Exception occured during loadSceneInformation.  Doublecheck your input file.  Cannot recover." << endl;
		//return(0);
	}

//Perform necessary preliminary calculations.
//Set up the viewing window etc.
	cout << "Starting Preliminary Calculations" << endl;
	pictureConfig.setViewingWindow();

	vector <vector <ColorType> > images = vector <vector <ColorType> > (9);
	
	pictureConfig.lights[0].center.y = 40;
	for(int i = 0; i < 9; i++)
	{

		pictureConfig.traceRays();
		images[i] = (pictureConfig.pixelArray);
		pictureConfig.lights[0].center.y = pictureConfig.lights[0].center.y - 8;

	}



	PictureData stitchedImage = stitchImages(images, pictureConfig.width, pictureConfig.height);
	stitchedImage.inputFileName = "ShiftLightSourceDemo.txt";
	//stitchedImages.outputFileName = "ShiftCameraDemo.ppm";


	
//Write the data to a image file.
	stitchedImage.writeImageFile();

}


//Shift the camera position a little.
void Demos::shiftCameraDemo()
{
	PictureData pictureConfig;

	pictureConfig.inputFileName = "bens_image.txt";
	//cout << "Input:" << pictureConfig.inputFileName << ":\n";

//Read input file to get the scene information.
	try
	{
		pictureConfig.loadSceneInformation();
	}
	catch(int e)
	{
		cout << "Exception occured during loadSceneInformation.  Doublecheck your input file.  Cannot recover." << endl;
		//return(0);
	}

//Perform necessary preliminary calculations.
//Set up the viewing window etc.
	cout << "Starting Preliminary Calculations" << endl;
	pictureConfig.setViewingWindow();

	vector <vector <ColorType> > images = vector <vector <ColorType> > (9);
	
	pictureConfig.eyeRay.origin.x = -1.0;
	pictureConfig.eyeRay.origin.y = 4.3;
	PointType originPoint;//0,0,0
	//pictureConfig.eyeRay.direction = pictureConfig.eyeRay.origin.vectorFromHereToPoint(originPoint);

	for(int i = 0; i < 9; i++)
	{
		//eye  0.0 5.0 10.0
		//viewdir  0.0 -5.0 -10.0
		pictureConfig.traceRays();
		images[i] = (pictureConfig.pixelArray);

		pictureConfig.eyeRay.origin.y = pictureConfig.eyeRay.origin.y + .15;
		pictureConfig.eyeRay.origin.x = pictureConfig.eyeRay.origin.x + .2;
		//pictureConfig.eyeRay.direction = pictureConfig.eyeRay.origin.vectorFromHereToPoint(originPoint);
	}

	cout << "VECTOR SIZE:" << images.size() << endl;


	PictureData stitchedImage = stitchImages(images, pictureConfig.width, pictureConfig.height);
	stitchedImage.inputFileName = "ShiftCameraDemo.txt";
	//stitchedImages.outputFileName = "ShiftCameraDemo.ppm";


	
//Write the data to a image file.
	stitchedImage.writeImageFile();

}


//Stitch together 9 images into one.
PictureData Demos::stitchImages(vector <vector <ColorType> > images, int width, int height)
{
	//cout << "starting to stitch images" << width << "," << height << endl;
	PictureData results;


	results.width = 3*width;
	results.height = 3*height;

	results.pixelArray = vector<ColorType>( (results.width) * (results.height) );

	//Loop through each pixel in the picture.
	//x and y are the coordinates on the big picture
	for(int y = 0; y < results.height ; y++)
	{
		for(int x = 0; x < results.width ; x++)
		{
			int localX = x % width;
			int localY = y % height;

			vector<ColorType> currentImage;
			if(y < height)
			{
				//top
				if(x < width)
					//left
					{results.pixelArray[ x + y * results.width ] = images[0][ localX + localY * width ];}			
				else if(x < 2 * width)
					//mid
					{results.pixelArray[ x + y * results.width ] = images[1][ localX + localY * width ];}	
				else
					//right
					{results.pixelArray[ x + y * results.width ] = images[2][ localX + localY * width ];}
			}
			else if(y < 2 * height)
			{
				if(x < width)
					{results.pixelArray[ x + y * results.width ] = images[3][ localX + localY * width ];}			
				else if(x < 2 * width)
					{results.pixelArray[ x + y * results.width ] = images[4][ localX + localY * width ];}	
				else
					{results.pixelArray[ x + y * results.width ] = images[5][ localX + localY * width ];}
			}			
			else
			{
				if(x < width)
					{results.pixelArray[ x + y * results.width ] = images[6][ localX + localY * width ];}			
				else if(x < 2 * width)
					{results.pixelArray[ x + y * results.width ] = images[7][ localX + localY * width ];}	
				else
					{results.pixelArray[ x + y * results.width ] = images[8][ localX + localY * width ];}
			}		
		}
	}

	//cout << "done stitching images" << endl;
	return results;

}
