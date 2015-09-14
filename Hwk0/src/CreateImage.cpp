/*
 * Ben Matern
 * Homework 0
 * Working with Ascii images
 * Computer Graphics
 *
 */

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	cout << "Creating a new image.\n";
	string inputFileName = "input/imageData.txt";
	string outputFileName = "output/outputImage.ppm";
	cout << "Input file = " << inputFileName << "\n";

	//"default" width and height
	int width = 100;
	int height = 100;

	//Read File
	string line;
	ifstream myInputfile (inputFileName);
	if (myInputfile.is_open())
	{
		while ( getline (myInputfile,line) )
	    {
			cout << line << '\n';
	    }
	    myInputfile.close();
	}
    else
    {
    	cout << "Unable to open file";
    }

	string widthString = to_string(width);
	string heightString = to_string(height);

	//Write File
	ofstream myfile;
	myfile.open (outputFileName);

	myfile << "P3\n";
	myfile << "# This image was created by Ben Matern from input file " << inputFileName << "\n";
	myfile << widthString + " " + heightString + "\n";
	myfile << "255\n";

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
			//Every 10 pixels horizontally has a green value of either 0 or 150.
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


