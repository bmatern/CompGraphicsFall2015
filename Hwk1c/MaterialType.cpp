#include "MaterialType.h"

void MaterialType::loadTexture()
{
	cout << "Loading Texture: " << textureFilename << endl;

	string line;
	ifstream myInputfile (textureFilename);
	if (myInputfile.is_open())
	{
		cout << "Texture file "<< textureFilename << " found.  Good.\n";
		string line1, line2, line3, line4;
		getline(myInputfile, line1);
		getline(myInputfile, line2);
		getline(myInputfile, line3);
		getline(myInputfile, line4);
		if(line1.compare("P3") != 0)
		{
			cout << "I expected P3 on the first line." << endl;
		}
		cout << "line1:" << line1 << endl;
		cout << "line2:" << line2 << endl;
		cout << "line3:" << line3 << endl;
		cout << "line4:" << line4 << endl;
		while ( getline (myInputfile,line) )
	    {

	    	//cout << "line:" << endl;
	    }
		myInputfile.close();
	}
    else
    {
    	cout << "Unable to open texture file.  \n";
		//-cout << "width:" << width << "\nheight:" << height <<  "\n";
    }


/*
	

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


*/
}
