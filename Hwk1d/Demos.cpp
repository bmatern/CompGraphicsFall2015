#include "Demos.h"

void Demos::generateTextures()
{
	ColorType white, black;
	white.r = 255;
	white.g = 255;
	white.b = 255;

	ColorType red,green,blue,yellow;
	red.r = 255;
	green.g = 255;
	blue.b = 255;

	yellow.r = 255;
	yellow.g = 255;

	ColorType lightYellow, lightBlue, lightGreen, lightRed;
	lightYellow.r = 255;
	lightYellow.g = 250;
	lightYellow.b = 205;

	lightBlue.b = 255;
	lightBlue.g = 191;

	lightGreen.r = 173;
	lightGreen.g = 255;
	lightGreen.b = 47;

	lightRed.r = 255;
	lightRed.g = 105;
	lightRed.b = 180;

	ColorType brown;
	brown.r = 139;
	brown.g = 69;
	brown.b = 19;


	cout << "Generating Textures..." << endl;

	//TEXTURE 1 is a checkerboard with black and white checkers.
	//The first one is a red checker.  Top left.
	//Top Right is BLUE.
	//lower left yellow.
	//lower right green.
	TextureType texture1;
	texture1.dimension = 9;
	texture1.textureFilename = "texture1.ppm";

	//the 1st pixel
	texture1.pixelArray.push_back(red);
	for(int i = 0; i < 40; i++)
	{
		texture1.pixelArray.push_back(white);
		//texture1.pixelArray[texture1.pixelArray.size() -1].r = i;
		texture1.pixelArray.push_back(black);
	}
	texture1.pixelArray[8] = blue;
	texture1.pixelArray[72] = yellow;
	texture1.pixelArray[80] = green;

	writeTexture(texture1);




	//Texture 2 is a 4x4 texture with 4 vertical lines, red, green, blue, yellow.
	TextureType texture2;
	texture2.dimension = 4;
	texture2.textureFilename = "texture2.ppm";


	for(int i = 0; i < 4; i++)
	{
		texture2.pixelArray.push_back(red);
		texture2.pixelArray.push_back(green);
		texture2.pixelArray.push_back(blue);
		texture2.pixelArray.push_back(yellow);
	}
	writeTexture(texture2);


	//Ok I'm gonna generate a skymap.  This should be fun.
	//top is blue and white.
	//bottom is green and brown.
	//side 1 is red and lightred.
	//side 2 is green and light green.
	//side 3 is yellow and light yellow.
	//side 4 is blue and lightblue.
	cout << "Generating the Skybox..." << endl;

	TextureType top, side1, side2, side3, side4, bottom;
	top.dimension = 9;
	top.textureFilename = "top.ppm";
	bottom.dimension = 9;
	bottom.textureFilename = "bottom.ppm";
	side1.dimension = 9;
	side1.textureFilename = "side1.ppm";
	side2.dimension = 9;
	side2.textureFilename = "side2.ppm";
	side3.dimension = 9;
	side3.textureFilename = "side3.ppm";
	side4.dimension = 9;
	side4.textureFilename = "side4.ppm";

	//the first pixel is black for every side.  
	top.pixelArray.push_back(black);
	bottom.pixelArray.push_back(black);
	side1.pixelArray.push_back(black);
	side2.pixelArray.push_back(black);
	side3.pixelArray.push_back(black);
	side4.pixelArray.push_back(black);
	for(int i = 0; i < 40; i++)
	{
		top.pixelArray.push_back(blue);
		top.pixelArray.push_back(white);

		bottom.pixelArray.push_back(green);
		bottom.pixelArray.push_back(brown);

		side1.pixelArray.push_back(red);
		side1.pixelArray.push_back(lightRed);

		side2.pixelArray.push_back(green);
		side2.pixelArray.push_back(lightGreen);

		side3.pixelArray.push_back(yellow);
		side3.pixelArray.push_back(lightYellow);

		side4.pixelArray.push_back(blue);
		side4.pixelArray.push_back(lightBlue);
	}

	writeTexture(top);
	writeTexture(bottom);
	writeTexture(side1);
	writeTexture(side2);
	writeTexture(side3);
	writeTexture(side4);

}

void Demos::writeTexture(TextureType inputTexture)
{
	cout << "Writing a texture file: " << inputTexture.textureFilename << endl;
	string widthString = to_string(inputTexture.dimension);
	//string heightString = to_string(height);

	string outputFileName = inputTexture.textureFilename;

	//Write image File
	ofstream myfile;
	myfile.open (outputFileName);

	//Write Header
	myfile << "P3\n";
	//myfile << "# This image was created by Ben Matern from input file " << inputFileName << "\n";
	myfile << widthString << " " << widthString << " 255" << endl;
	//myfile << "255\n";

	//Loop through each pixel in the picture.
	for(int i = 0; i < inputTexture.pixelArray.size(); i++)
	//for(int y = 0; y < inputTexture.dimension ; y++)
	{
		//for(int x = 0; x < inputTexture.dimension ; x++)
		{
			int r_ = round(inputTexture.pixelArray[i].r);
			int g_ = round(inputTexture.pixelArray[i].g);
			int b_ = round(inputTexture.pixelArray[i].b);
			string r = to_string(r_) + " ";
			string g = to_string(g_) + " ";
			string b = to_string(b_) + " ";
			//string b = Common::getColorValue(inputTexture.pixelArray[ x + y*inputTexture.dimension ].b);
			//string g = Common::getColorValue(inputTexture.pixelArray[ x + y*inputTexture.dimension ].g);
			myfile << r << g << b;
			myfile << "\n";
		}
		//We just finished a row of pixels.
		//myfile << "\n";
	}

	myfile << "";
	myfile.close();
}

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

		/*pictureConfig.materials[pictureConfig.spheres[1].materialIndex].ka = pictureConfig.materials[pictureConfig.spheres[1].materialIndex].ka * .8;
		pictureConfig.materials[pictureConfig.spheres[2].materialIndex].ka = pictureConfig.materials[pictureConfig.spheres[2].materialIndex].ka * .8;
		pictureConfig.materials[pictureConfig.spheres[3].materialIndex].kd = pictureConfig.materials[pictureConfig.spheres[3].materialIndex].kd * .8;
		pictureConfig.materials[pictureConfig.spheres[4].materialIndex].kd = pictureConfig.materials[pictureConfig.spheres[4].materialIndex].kd * .8;
		pictureConfig.materials[pictureConfig.spheres[5].materialIndex].ks = pictureConfig.materials[pictureConfig.spheres[5].materialIndex].ks * .8;
		pictureConfig.materials[pictureConfig.spheres[6].materialIndex].ks = pictureConfig.materials[pictureConfig.spheres[6].materialIndex].ks * .8;	*/
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
