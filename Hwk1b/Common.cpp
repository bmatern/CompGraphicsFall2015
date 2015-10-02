//#include "VectorType.h"

#include "Common.h"

using namespace std;

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

void Common::writeImageFile(ColorType pixelArray[], int width, int height, string inputFileName)
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