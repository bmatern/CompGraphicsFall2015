/*
 * Ben Matern
 * Homework 1b
 * Light and Shadow
 * Computer Graphics
 *
 * This program can create a ASCII Format image from an input file.
 * I'm getting better at structuring c++
 * Instincts tell me I should break up this file quite a bit.
 * I'll have to figure out file includes for the next assignment.
 * Main method is at the very bottom.
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "PointType.h"
using namespace std;


typedef struct 
{
	PointType origin;
	PointType direction;
} RayType;

typedef struct 
{
	double r, g, b;
} ColorType;

typedef struct 
{
	PointType center;
	double radius;
	ColorType color;
} SphereType;

typedef struct
{
	PointType topLeft;
	PointType topRight;
	PointType botLeft;
	PointType botRight;
} ViewingWindowType;

double dotProduct(PointType firstVector, PointType secondVector)
{
	//Calculate dot product of 2 vectors
	//s = x1 x2 + y1 y2 + z1 z2
	double results = 
		firstVector.x * secondVector.x +
		firstVector.y * secondVector.y +
		firstVector.z * secondVector.z;
	return results;
}

PointType crossProduct(PointType firstVector, PointType secondVector)
{
	//calculate cross product firstVector X secondVector
	//u x v = 
		// (uy * vz - uz * vy 
		//, uz * vx - ux * vz 
		//, ux * vy - uy * vx)
	PointType results;
	results.x = (firstVector.y * secondVector.z) - (firstVector.z * secondVector.y);
	results.y = (firstVector.z * secondVector.x) - (firstVector.x * secondVector.z);
	results.z = (firstVector.x * secondVector.y) - (firstVector.y * secondVector.x);
	return results;
}

PointType addVectors(PointType firstVector, PointType secondVector)
{
	//respectively add the components of the two vectors.
	PointType results;
	results.x = firstVector.x + secondVector.x;
	results.y = firstVector.y + secondVector.y;
	results.z = firstVector.z + secondVector.z;
	return results;
}

PointType subtractPoints(PointType firstPoint, PointType secondPoint)
{
	//subtract the first from the second.
	//This finds the vector from firstPoint -> secondPoint
	PointType results;
	results.x = secondPoint.x - firstPoint.x;
	results.y = secondPoint.y - firstPoint.y;
	results.z = secondPoint.z - firstPoint.z;
	return results;
}

PointType multiplyVector(PointType inputVector, double scalar)
{
	//Multiply a vector by a scalar
	PointType results;	
	results.x = inputVector.x * scalar;
	results.y = inputVector.y * scalar;
	results.z = inputVector.z * scalar;	
	return results;
}

double vectorLength(PointType inputVector)
{
	//calculate the length of a vector
	//sqrt(x^2 + y^2 + z^2)
	return sqrt(inputVector.x * inputVector.x
		+ inputVector.y * inputVector.y
		+ inputVector.z * inputVector.z);
}

PointType normalizeVector(PointType inputVector)
{
	//normalize the vector to length 1.
	double normalizeScalar = 1.0 / vectorLength(inputVector);
	PointType results = multiplyVector(inputVector, normalizeScalar);
	return results;
}

string trim(string str)
{
	//trim whitespace from the beginning and end of a string.
	//cout << "trimming:" << str <<"\n";
	size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    string result = str.substr(first, (last-first+1));
    //cout << "result:" << result <<"\n";
    return result;
}

vector<string> tokenizeString(string input)
{
	//split a string by a space delimiter.
	//returns a vector of token strings.
	vector<string> tokens;
	int spaceLocation;
	string tokenBuffer = trim(input);
	
	while(true)
	{
		//cout << "tokenizing this:" << tokenBuffer << "\n";
		spaceLocation = tokenBuffer.find(" ");
		if(spaceLocation < 1)
		{
			if(tokenBuffer.length() > 0)
			{
				//cout << "Last token:" << tokenBuffer << "\n";
				tokens.push_back(tokenBuffer);
			}
			//cout << "End of tokens.\n";
			break;
		}
		else
		{
			string token = tokenBuffer.substr(0, spaceLocation);
			//cout << "Token found:" << token << "\n";
			tokenBuffer = tokenBuffer.substr(spaceLocation + 1, tokenBuffer.length());
			tokenBuffer = trim(tokenBuffer);
			//cout << "new Buffer:" << tokenBuffer << "\n";
			
			tokens.push_back(token);
		}
	}
	return tokens;
}

string getInputFileName(int argc, char *argv[])
{
	//Read commandline args to get input filename.
	string inputFileName;
	if ( argc == 2 )
	{
		//cout << "arg0:" << argv[0];
		inputFileName = argv[1];
		cout << "one argument found.  Good.  Filename = " << inputFileName << "\n";
	}
	else
	{
		cout << "You should have exactly one argument, the input file name.  Using default filename instead.\n";
		inputFileName = "example_input.txt";
	}
	return inputFileName;
}

void loadSceneInformation(string inputFileName, RayType& eyeRay, PointType& upVector, int& width, int& height, double& fovH
	, ColorType& bgColor, ColorType& currentMaterialColor, vector<SphereType>& spheres)
{
	//read input file to determine the components of the scene.
	string line;
	ifstream myInputfile (inputFileName);
	if (myInputfile.is_open())
	{
		cout << "Input File Found.  Good.\n";
		while ( getline (myInputfile,line) )
	    {
	    	//cout << "Line:" << line << "\n";
	    	if(line.length() > 0)
	    	{
	    		    	
				vector<string> tokens = tokenizeString(line);
				if(tokens.size() > 0)
				{
					//cout <<	tokens.size() << " tokens found.\n";		
					string inputVarName = tokens[0];
					//cout << "Found:" << inputVarName << "\n";
					
					if(inputVarName.compare("eye") == 0)
					{
						eyeRay.origin.x = stof(tokens[1]);
						eyeRay.origin.y = stof(tokens[2]);
						eyeRay.origin.z = stof(tokens[3]);			

					}		
					else if(inputVarName.compare("viewdir") == 0)
					{
						eyeRay.direction.x = stof(tokens[1]);
						eyeRay.direction.y = stof(tokens[2]);
						eyeRay.direction.z = stof(tokens[3]);
						if(vectorLength(eyeRay.direction) == 0)	
						{
							cout << "View direction vector has length 0.  Please fix the input file." << endl;
							throw(1);
						}

					}
					else if(inputVarName.compare("updir") == 0)
					{
						upVector.x = stof(tokens[1]);
						upVector.y = stof(tokens[2]);
						upVector.z = stof(tokens[3]);

						if(vectorLength(upVector) == 0)	
						{
							cout << "Up vector has length 0.  Please fix the input file." << endl;
							throw(1);
						}
					
					}
					else if(inputVarName.compare("fovh") == 0)
					{
						fovH = stof(tokens[1]);
						if(vectorLength(upVector) > 180)	
						{
							cout << "fovh should be <= 180.  Please fix the input file." << endl;
							throw(1);
						}
					}
					else if(inputVarName.compare("imsize") == 0)
					{
						width = stoi(tokens[1]);
						height = stoi(tokens[2]);
					}
					else if(inputVarName.compare("bkgcolor") == 0)
					{
						bgColor.r = stof(tokens[1]);
						bgColor.g = stof(tokens[2]);
						bgColor.b = stof(tokens[3]);
					}
					else if(inputVarName.compare("mtlcolor") == 0)
					{
						currentMaterialColor.r = stof(tokens[1]);
						currentMaterialColor.g = stof(tokens[2]);
						currentMaterialColor.b = stof(tokens[3]);
					}
					else if(inputVarName.compare("sphere") == 0)
					{
						SphereType currentSphere;
							
						currentSphere.center.x = stof(tokens[1]);
						currentSphere.center.y = stof(tokens[2]);
						currentSphere.center.z = stof(tokens[3]);
						currentSphere.radius = stof(tokens[4]);
						currentSphere.color.r = currentMaterialColor.r;
						currentSphere.color.g = currentMaterialColor.g;
						currentSphere.color.b = currentMaterialColor.b;
					
						spheres.push_back(currentSphere);					
					}
					else
					{
						cout << "Something's wrong.  Unidentified input:" << inputVarName << "\n";
					}
				}
				else
				{
					cout << "No tokens on this line.\n";
				}
			}
			//break;
	    }
	    myInputfile.close();
	}
    else
    {
    	//TODO: Defaults.   Probably need to default a few more things here.
    	width = 100;
    	height = 100;
    	cout << "Unable to open input file. Using default image size values.\n";
		cout << "width:" << width << "\nheight:" << height <<  "\n";
    }
}

void setViewingWindow(ViewingWindowType& viewingWindow, RayType eyeRay, PointType upVector, int width, int height, double fovH
	, double& d, double& w, double& h , PointType& u, PointType& v)
{
	//calculate the viewing window parameters.
	//Mostly we want to find out the u and v vectors, and the 
	//4 corners of the window.

	//the u vector is the unit vector orthogonal to the viewing direction and the up direction.
	// u' is the non-normalized u vector.
	// view_dir x up_dir = u'
	// u = u'/ ||u'||	
	PointType uPrime = crossProduct(eyeRay.direction , upVector);
	u = normalizeVector(uPrime);
	
	//v is a unit vector orthogonal to both the viewing direction and u.
	PointType vPrime = crossProduct(u , eyeRay.direction);
	v = normalizeVector(vPrime);
	
	// d is an arbitrary viewing distance, from eye to viewing window.  It is in world coordinates.
	d = 1.0;
	// w is the width of the viewing window.  In 3d world coordinate units.
	// I need to calculate w based on the tangent of fovH and d.  Algebra and trig yield this equation:
	// w = 2 * d * tan (fovH / 2)
	w = 2.0 * d * tan (fovH / 2);
	// calculate h, in world coordinates.  Use the aspect ratio calculated from the width and height in pixels.
	h = w * height / width ;
	
	//n is the normalized view direction vector
	PointType n = normalizeVector(eyeRay.direction);
	
	//each of these calculations are one point + 3 vectors.
	//This results in a point (well, 4 points)
	//ul = view_origin + d⋅n + h/2⋅v – w/2⋅u
	//ur = view_origin + d⋅n + h/2⋅v + w/2⋅u
	//ll = view_origin + d⋅n – h/2⋅v – w/2⋅u
	//lr = view_origin + d⋅n – h/2⋅v + w/2⋅u
	viewingWindow.topLeft = addVectors(addVectors(addVectors(
		eyeRay.origin 
		, multiplyVector(n, d))
		, multiplyVector(v, h/2))
		, multiplyVector(u, -w/2));		
	viewingWindow.topRight = addVectors(addVectors(addVectors(
		eyeRay.origin 
		, multiplyVector(n, d))
		, multiplyVector(v, h/2))
		, multiplyVector(u, w/2));
	viewingWindow.botLeft = addVectors(addVectors(addVectors(
		eyeRay.origin 
		, multiplyVector(n, d))
		, multiplyVector(v, -h/2))
		, multiplyVector(u, -w/2));		
	viewingWindow.botRight = addVectors(addVectors(addVectors(
		eyeRay.origin 
		, multiplyVector(n, d))
		, multiplyVector(v, -h/2))
		, multiplyVector(u, w/2));
			
	cout << "Double check the aspect ratios to see that they match." << endl;
	double pixAspRat = 1.0 * width /height;
	cout << "Pixel aspect ratio:" << pixAspRat << endl;
	double worldAspRat = (vectorLength(addVectors(viewingWindow.botLeft, viewingWindow.botRight)))
		/ (vectorLength(subtractPoints(viewingWindow.botLeft, viewingWindow.topLeft)));
	cout << "World aspect ratio:" << pixAspRat << endl;

}

double intersectSphere(RayType inputRay, SphereType inputSphere)
{
	//find the location of the intersection between a ray and a sphere.
	//return the distance to the intersection.
	//return -1 if there is no intersection.
	//Solving the combined sphere and ray equation to find an intersection.
	//For the quadratic equation
	//A = (xd^2 + yd^2 + zd^2) = 1;
	//B = 2*(xd * (x0 - xc) + yd*(y0-yc) + zd*(z0-zc))
	//C = (x0-xc)^2 + (y0-yc)^2 + (z0-zc)^2 -r^2
	
	double A = 1;
	double B = 2 * (
		inputRay.direction.x * (inputRay.origin.x - inputSphere.center.x)
		+ inputRay.direction.y * (inputRay.origin.y - inputSphere.center.y)
		+ inputRay.direction.z * (inputRay.origin.z - inputSphere.center.z));
	double C = 
		pow( inputRay.origin.x - inputSphere.center.x , 2)
		+ pow( inputRay.origin.y - inputSphere.center.y , 2)
		+ pow( inputRay.origin.z - inputSphere.center.z , 2)
		- pow( inputSphere.radius , 2);
		
	//calculate the quadratic equation's discriminant, (B^2 - 4AC)
	double discriminant = pow(B,2) - (4*A*C);
	if (discriminant > 0)
	{
		//cout << "2 solutions found" << endl;
		//There are 2 solutions.  t is the "time" until the ray hits the object.
		//We want the minimum positive t
		double t1 = (-B + sqrt(discriminant))/(2*A);
		double t2 = (-B - sqrt(discriminant))/(2*A);
		//t<0 means that the object is behind the eye.  Can't see it.
		if(t1 < 0)
		{
			return (t2<0) ? -1 : t2;
		}
		else if(t2<0)
		{
			//Can't see t2
			return t1;
		}
		else
		{
			//return the minimum of the two positive solutions.  That's the closest intersection.
			return ( t1<t2 ? t1 : t2);
		}
	}
	else if (discriminant == 0)
	{
		//cout << "1 solution found" << endl;
		//Don't reckon this'll happen much.  Graze the edge of the sphere.  1 solution.
		//Discriminant cancelled out.
		double t = (-B)/(2*A);
		return (t>0) ? t : -1;
	}
	else
	{
		//discriminant is negative.  No solution.  No intersection.
		return -1;
	}
}

ColorType trace_ray(ViewingWindowType viewingWindow, RayType eyeRay, int x, int y, PointType deltaH, PointType deltaV
	, ColorType bgColor, vector<SphereType> spheres)
{
	//default to background color.  Switch to sphere color if ray intersects it.
	ColorType results = bgColor;	
	//This is the mapping to the world coordinate point.
	//topleft + deltaH*x + deltaV * y
	PointType worldCoordinatesPoint = addVectors(addVectors(
		viewingWindow.topLeft
		,multiplyVector(deltaH, x))
		,multiplyVector(deltaV, y));
		
	RayType rayToTrace;
	rayToTrace.origin = eyeRay.origin;
	rayToTrace.direction = normalizeVector(subtractPoints(rayToTrace.origin, worldCoordinatesPoint));
			
	//determine intersection points with each sphere.
	double intersectDistance = -1;
	for(int i = 0; i < spheres.size(); i++)
	{
		double currentIntersectDistance = intersectSphere(rayToTrace, spheres[i]);
		if(currentIntersectDistance != -1 && 
			(currentIntersectDistance < intersectDistance || intersectDistance == -1))
		{
			results = spheres[i].color;
		}	
	}
			
			
	return results;
}

void writeImageFile(ColorType pixelArray[], int width, int height, string inputFileName)
{
	string widthString = to_string(width);
	string heightString = to_string(height);

	string outputFileName = inputFileName.substr(0,inputFileName.length()-4) + ".ppm";

	//Write image File
	ofstream myfile;
	myfile.open (outputFileName);

	//Write Header
	myfile << "P3\n";
	myfile << "# This image was created by Ben Matern from input file " << inputFileName << "\n";
	myfile << widthString + " " + heightString + "\n";
	myfile << "1\n";

	//Loop through each pixel in the picture.
	for(int y = 0; y < height ; y++)
	{
		for(int x = 0; x < width ; x++)
		{
			string r = to_string(pixelArray[ x + y*width ].r) + " ";
			string b = to_string(pixelArray[ x + y*width ].b) + " ";
			string g = to_string(pixelArray[ x + y*width ].g) + " ";
			myfile << r << g << b;
		}
		//We just finished a row of pixels.
		myfile << "\n";
	}

	myfile << "";
	myfile.close();
}

int main( int argc, char *argv[] )
{
	cout << "Creating a new image.\n";

//Declare all the important variables I'll need
	
	//This width and height are in pixels.
	int width, height;
	
	//d, w, h are in world coordinates
	//d = distance from eye to window.  w=width window, h = height window.
	double d, w, h;
	
	RayType eyeRay;	
	
	PointType upVector;
	PointType u;
	PointType v;	
	
	ColorType bgColor;
	ColorType currentMaterialColor;
	
	vector<SphereType> spheres;
	
	//The angle of the horizontal field of view.  Theta.
	double fovH;
	
	//contains 4 points for the coordinates of the viewing window.
	ViewingWindowType viewingWindow;

	string inputFileName;

//Read commandline args to get the input file name.
	inputFileName = getInputFileName(argc, argv);
	cout << "Input:" << inputFileName << ":\n";

//Read input file to get the scene information.
	try
	{
		loadSceneInformation(inputFileName, eyeRay, upVector, width, height, fovH
			, bgColor, currentMaterialColor, spheres);
	}
	catch(int e)
	{
		cout << "Exception occured during loadSceneInformation.  Doublecheck your input file.  Cannot recover." << endl;
		return(0);
	}

//Initialize pixel array for output image
	cout << "Initializing Pixel Array" << endl;
	ColorType* pixelArray =new ColorType[width*height];

//Perform necessary preliminary calculations.
	cout << "Starting Preliminary Calculations" << endl;
	setViewingWindow(viewingWindow, eyeRay, upVector, width, height
		, fovH, d, w, h ,u , v);

//foreach pixel in image array:
	//call trace_ray() with appropriate parameters
	//Use value returend by trace_ray to update pixel colors.
	cout << "Starting the loop through each pixel" << endl;
	//Passing in deltaH and deltaV so I don't need to calculate it every time.
	//deltaH and deltaV are VECTORS.  they indicate how far to increment the viewing window for each pixel.
	//deltaH = (ur - ul)/(pixwidth - 1)
	PointType deltaH = multiplyVector( subtractPoints(viewingWindow.topLeft, viewingWindow.topRight) 
		, (1.0/(width - 1)));
	PointType deltaV = multiplyVector( subtractPoints(viewingWindow.topLeft, viewingWindow.botLeft) 
		, (1.0/(height - 1)));

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			ColorType currentPixel = trace_ray(viewingWindow, eyeRay, x, y, deltaH, deltaV, bgColor, spheres);
			pixelArray [ x + y*width ] = currentPixel;
		}
	}
	
//Write the data to a image file.
	writeImageFile(pixelArray, width, height, inputFileName);

	return 0;
}


