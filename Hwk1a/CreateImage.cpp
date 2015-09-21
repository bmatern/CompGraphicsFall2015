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
using namespace std;

int main( int argc, char *argv[] )
{
	cout << "Creating a new image.\n";

	int width;
	int height;
	
	double eyeX;
	double eyeY;
	double eyeZ;
	
	double viewDirX;
	double viewDirY;
	double viewDirZ;

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
		inputFileName = "imageData.txt";
	}

	//this is crap but I'm not very good at string operations in c++ right now :D
	outputFileName = inputFileName.substr(0,inputFileName.length()-4) + ".ppm";

	cout << "Input:" << inputFileName << "\n";
	cout << "Output:" << outputFileName << "\n";

	//Read File
	//I'm expecting an input file with a single line of text.
	//The text will look like this:
	//imsize 250 500
	//I'm not sure why i need the text "imsize" at the beginning.
	//I will parse out the width and height from this input.
	string line;
	ifstream myInputfile (inputFileName);
	if (myInputfile.is_open())
	{
		while ( getline (myInputfile,line) )
	    {
			//This is some ghetto string parsing.  Im sure there are better ways to do it.
			int firstSpace = line.find(" ");
			//string imSizeString = line.substr(0,firstSpace);
			string remainder = line.substr(firstSpace + 1, line.length());
			int secondSpace = remainder.find(" ");

			width = stoi(remainder.substr(0,secondSpace));
			height = stoi(remainder.substr(secondSpace + 1, remainder.length()));
			cout << "width:" << width << "\nheight:" << height <<  "\n";

			break;
	    }
	    myInputfile.close();
	}
    else
    {
    	width = 100;
    	height = 100;
    	cout << "Unable to open input file. Using default image size values.\n";
		cout << "width:" << width << "\nheight:" << height <<  "\n";
    }

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


