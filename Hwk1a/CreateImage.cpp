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
using namespace std;

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

string trim(string& str)
{
	//cout << "trimming:" << str <<"\n";
	size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    string result = str.substr(first, (last-first+1));
    //cout << "result:" << result <<"\n";
    return result;
}

vector<string> tokenizeString(string& input)
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

	int width;
	int height;
	
	RayType eyeRay;	
	RayType upVector;
	ColorType bgColor;
	ColorType currentMaterialColor;
	
	vector<SphereType> spheres;
	
	float fovH;

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
				
						upVector.origin.x =	0; 
						upVector.origin.y = 0;
						upVector.origin.z = 0;
						upVector.direction.x = stof(tokens[1]);
						upVector.direction.y = stof(tokens[2]);
						upVector.direction.z = stof(tokens[3]);
					
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
						cout << "What else????\n";
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
    	//Defaults.  
    	width = 100;
    	height = 100;
    	cout << "Unable to open input file. Using default image size values.\n";
		cout << "width:" << width << "\nheight:" << height <<  "\n";
    }




	cout << "How many spheres?" << spheres.size() << "\n";
	cout << "sphere 3 x" << spheres[2].center.x << "\n";
	cout << "sphere 3 r" << spheres[2].color.r << "\n";












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
			//Red and blue values are calculated to form a gradient.
			//Red increases with the X value, to a maximum of 255.
			//Blue increases with the Y value, to a maximum of 255.
			string r = to_string((255.0 * x) / width) + " ";
			string b = to_string((255.0 * y) / height) + " ";

			//I decided to calculate green based on a repeating pattern every 10 pixels.
			//Every set of 10 pixels horizontally has a green value of either 0 or 150.
			//Looks real ugly that way.
			bool isOdd = ((x/10) % 2==1);
			string g = isOdd?("0 "):"150 ";
			myfile << r << g << b;
		}
		//We just finished a row of pixels.
		myfile << "\n";
	}

	myfile << "";
	myfile.close();

	return 0;
}


