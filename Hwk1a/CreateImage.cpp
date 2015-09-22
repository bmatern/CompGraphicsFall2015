/*
 * Ben Matern
 * Homework 1a
 * Getting started with raycasting
 * Computer Graphics
 *
 * This program can create a ASCII Format image from an input file.
 * Sorry for spaghetti code, haven't written c++ in a decade or so :D
 * Most of my structure habits are from java.
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

//PointType can double as a vector too.  It's just numbers.  Let's be reasonable.
//It's not the same as a c++ vector.  Or a virology vector.  How confusing.
typedef struct
{
	float x,y,z;
} PointType;

typedef struct 
{
	PointType origin;
	PointType direction;
} RayType;

typedef struct 
{
	float r, g, b;
} ColorType;

typedef struct 
{
	PointType center;
	float radius;
	ColorType color;
} SphereType;

typedef struct
{
	PointType topLeft;
	PointType topRight;
	PointType botLeft;
	PointType botRight;
} ViewingWindowType;

float dotProduct(PointType firstVector, PointType secondVector)
{
	//s = x1 x2 + y1 y2 + z1 z2
	float results = 
		firstVector.x * secondVector.x +
		firstVector.y * secondVector.y +
		firstVector.z * secondVector.z;
	return results;
}

PointType crossProduct(PointType firstVector, PointType secondVector)
{
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
	PointType results;
	results.x = firstVector.x + secondVector.x;
	results.y = firstVector.y + secondVector.y;
	results.z = firstVector.z + secondVector.z;
	return results;
}

PointType multiplyVector(PointType inputVector, float scalar)
{
	PointType results;	
	results.x = inputVector.x * scalar;
	results.y = inputVector.y * scalar;
	results.z = inputVector.z * scalar;	
	return results;
}

float vectorLength(PointType inputVector)
{
	//sqrt(x^2 + y^2 + z^2)
	return sqrt(inputVector.x * inputVector.x
		+ inputVector.y * inputVector.y
		+ inputVector.z * inputVector.z);
}

PointType normalizeVector(PointType inputVector)
{
	float normalizeScalar = 1.0 / vectorLength(inputVector);
	PointType results = multiplyVector(inputVector, normalizeScalar);
	return results;
}

string trim(string str)
{
	//cout << "trimming:" << str <<"\n";
	size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    string result = str.substr(first, (last-first+1));
    //cout << "result:" << result <<"\n";
    return result;
}

vector<string> tokenizeString(string input)
{
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

int main( int argc, char *argv[] )
{
	cout << "Creating a new image.\n";

//Declare all the important variables I'll need
	
	//This width and height are in pixels.
	int width, height;
	
	//d, w, h are in world coordinates
	//d = distance from eye to window.  w=width window, h = height window.
	float d, w, h;
	
	RayType eyeRay;	
	
	PointType upVector;
	PointType u;
	PointType v;	
	PointType n;
	
	ColorType bgColor;
	ColorType currentMaterialColor;
	
	vector<SphereType> spheres;
	
	//The angle of the horizontal field of view.  Theta.
	float fovH;
	
	//contains 4 points for the coordinates of the viewing window.
	ViewingWindowType viewingWindow;

	string inputFileName;
	string outputFileName;

//Read commandline args to get the input file name.
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

	//this is crap but I'm not very good at string operations in c++ right now :D
	outputFileName = inputFileName.substr(0,inputFileName.length()-4) + ".ppm";

	cout << "Input:" << inputFileName << ":\n";
	cout << "Output:" << outputFileName << ":\n";

//Read input file to get the scene information.
	string line;

	//TODO This should be dynamic for inputFileName passed in.  Instead of text literal.  IDK.  Pointers?
	//Maybe I need to do a *symbol to dereference something.  I don't know.
	ifstream myInputfile ("example_input.txt");
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

					}
					else if(inputVarName.compare("updir") == 0)
					{
						upVector.x = stof(tokens[1]);
						upVector.y = stof(tokens[2]);
						upVector.z = stof(tokens[3]);
					
					}
					else if(inputVarName.compare("fovh") == 0)
					{
						fovH = stof(tokens[1]);
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




//Initialize pixel array for output image
	ColorType pixelArray [width][height]; 

//Perform necessary preliminary calculations.
	//I think this is where I define the viewing window?
	//Yeah I need the u and v vectors.  And the corners of the viewing window.
	
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
	n = normalizeVector(eyeRay.direction);
	
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
		
		
		
		







//foreach pixel in image array:
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			ColorType currentPixel;
			currentPixel.r = 3;
			currentPixel.r = 4;
			currentPixel.r = 200;
			pixelArray [x][y] = currentPixel;
			//call trace_ray() with appropriate parameters
			//Use value returend by trace_ray to update pixel colors.
		}
	}
	
	



//Write the data to a image file.

	string widthString = to_string(width);
	string heightString = to_string(height);

	//Write image File
	ofstream myfile;
	myfile.open (outputFileName);

	//Write Header
	myfile << "P3\n";
	myfile << "# This image was created by Ben Matern from input file " << inputFileName << "\n";
	myfile << widthString + " " + heightString + "\n";
	myfile << "255\n";

	//Loop through each pixel in the picture.
	for(int y = 0; y < height ; y++)
	{
		for(int x = 0; x < width ; x++)
		{
			string r = to_string(pixelArray[x][y].r) + " ";
			string b = to_string(pixelArray[x][y].b) + " ";
			string g = to_string(pixelArray[x][y].g) + " ";
			myfile << r << g << b;
		}
		//We just finished a row of pixels.
		myfile << "\n";
	}

	myfile << "";
	myfile.close();

	return 0;
}


