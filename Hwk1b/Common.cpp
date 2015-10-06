#include "Common.h"

double Common::intersectSphere(RayType inputRay, SphereType inputSphere)
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

string Common::trim(string str)
{
	//trim whitespace from the beginning and end of a string.
	//cout << "trimming:" << str <<"\n";
	size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    string result = str.substr(first, (last-first+1));
    //cout << "result:" << result <<"\n";
    return result;
}

vector<string> Common::tokenizeString(string input)
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

string Common::getInputFileName(int argc, char *argv[])
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

string Common::getColorValue(double colorValue)
{
	//to_string(inputPicture.pixelArray[ x + y*inputPicture.width ].r) + " ";
	double multiplied = colorValue * 255.0;
	int roundedInt = round(multiplied);
	//cout << "GettingColorValue " << colorValue << "," << multiplied << "," << roundedInt << endl;
	return to_string(roundedInt) + " ";
}

void Common::writeImageFile(PictureData inputPicture)
{
	string widthString = to_string(inputPicture.width);
	string heightString = to_string(inputPicture.height);

	string outputFileName = inputPicture.inputFileName.substr(0,inputPicture.inputFileName.length()-4) + ".ppm";

	//Write image File
	ofstream myfile;
	myfile.open (outputFileName);

	//Write Header
	myfile << "P3\n";
	myfile << "# This image was created by Ben Matern from input file " << inputPicture.inputFileName << "\n";
	myfile << widthString + " " + heightString + "\n";
	myfile << "255\n";

	//Loop through each pixel in the picture.
	for(int y = 0; y < inputPicture.height ; y++)
	{
		for(int x = 0; x < inputPicture.width ; x++)
		{
			string r = getColorValue(inputPicture.pixelArray[ x + y*inputPicture.width ].r);
			string b = getColorValue(inputPicture.pixelArray[ x + y*inputPicture.width ].b);
			string g = getColorValue(inputPicture.pixelArray[ x + y*inputPicture.width ].g);
			myfile << r << g << b;
		}
		//We just finished a row of pixels.
		myfile << "\n";
	}

	myfile << "";
	myfile.close();
}

bool Common::thresholdCompare(double a, double b)
{
	//Matt suggested we use a threshold compare, rather than direct equals.
	//This is because floating point values are silly, and represnt numbers in funny ways.
	//This method will return true if (a-b < episolon)
	//which means a and b are close enough to be considered equal.
	//Pick a small epsilon as a threshold
	//I apparenlty arbitrarily chose 10^-6
	double epsilon = .000001;
	return (a-b < epsilon);
}