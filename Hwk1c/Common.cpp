#include "Common.h"

PointType Common::getPointFromVector(VectorType inputVector)
{
	//This is the point at the tip of a vector, if assumed to originate at 0,0,0
	PointType results;
	results.x = inputVector.x;
	results.y = inputVector.y;
	results.z = inputVector.z;
	return results;
}

VectorType Common::getVectorFromPoint(PointType inputPoint)
{
	//This is the vector from the origin to the input point.
	VectorType results;
	results.x = inputPoint.x;
	results.y = inputPoint.y;
	results.z = inputPoint.z;
	return results;
}

VectorType Common::vectorFromHereToPoint(PointType firstPoint, PointType secondPoint)
{
	//find the vector from THIS point, to the second point.
	VectorType results;
	results.x = secondPoint.x - firstPoint.x;
	results.y = secondPoint.y - firstPoint.y;
	results.z = secondPoint.z - firstPoint.z;
	return results;
}


TextureType Common::loadTexture(string filename)
{
	TextureType results;
	results.textureFilename = filename;
	//cout << "Loading Texture: " << results.textureFilename << endl;

	string line;
	ifstream myInputfile (results.textureFilename);
	if (myInputfile.is_open())
	{
		//cout << "Texture file "<< results.textureFilename << " found.  Good.\n";
		string line1, line2;//, line3, line4;
		getline(myInputfile, line1);
		getline(myInputfile, line2);
		//getline(myInputfile, line3);
		//getline(myInputfile, line4);
		if(line1.compare("P3") != 0)
		{
			cout << "I expected P3 on the first line." << endl;
		}
		//cout << "line1:" << line1 << endl;
		//cout << "line2:" << line2 << endl;

		vector<string> paramTokens = tokenizeString(line2, " ");
		int width = stoi(paramTokens[0]);
		int height = stoi(paramTokens[1]);
		int maxColorValue = stoi(paramTokens[2]);
		//cout << "width:" << width << ",height:" << height << ",maxColor:"<<maxColorValue << endl;

		if(width != height)
		{
			cout <<"width and height for the texture should be equal.  I'm making a square using the smaller of the dimensions." << endl;
			results.dimension = (width < height)?width:height;
		}
		else
		{
			//cout <<"width and height are equal.  Good." << endl;
			results.dimension = height;
		}
		//cout << "line3:" << line3 << endl;
		//cout << "line4:" << line4 << endl;
		while ( getline (myInputfile,line) )
	    {
	    	vector<string> tokens = tokenizeString(line, " ");
	    	int tokenCount = tokens.size() ;
	    	//cout << "tokencount:"  << tokenCount << endl;

	    	//Pull out the RGB values from the texture file.
	    	for(int i = 0; i < tokenCount; i=i+3)
	    	{
	    		ColorType currentPixel;
	    		currentPixel.r = stod(tokens[i]);
	    		currentPixel.g = stod(tokens[i+1]);
	    		currentPixel.b = stod(tokens[i+2]);
	    		
	    		results.pixelArray.push_back(currentPixel);
	    	}
	    	//Common::t
	    	//cout << "Done"  << tokenCount << endl;

	    	//cout << "line:" << endl;
	    }
	   //cout <<"Number Pixels in Texture: " << results.pixelArray.size() << endl;
	    //cout<<"Closing Input"<<endl;
		myInputfile.close();
	}
    else
    {
    	cout << "Unable to open texture file.  \n";
		//-cout << "width:" << width << "\nheight:" << height <<  "\n";
    }

    return results;

}

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
	else if (thresholdEquals(discriminant,0))
	{
		//cout << "1 solution found" << endl;
		//Don't reckon this'll happen much.  Graze the edge of the sphere.  1 solution.
		//Discriminant cancelled out.
		double t = (-B)/(2*A);
		return (t>0) ? t : -1;
	}
	else
	{
		//cout <<"NO INTERSECTION, INTERSECT DISTANCE SHOULD BE -1" << endl;
		//discriminant is negative.  No solution.  No intersection.
		return -1;
	}
}

double Common::intersectFace(RayType inputRay, FaceType inputFace)
{
	//cout <<"Attempting a face intersection" << endl;
	//find the location of the intersection between a ray and a triangle face.
	//return the distance to the intersection.
	//return -1 if there is no intersection.

	//determine plane equation.
	//Let the vector e1 be defined along the edge from p0 to p1
	//Let the vector e2 be defined along the edge from p0 to p2
	//Let the vector n be defined by the cross product of e1 and e2
	//That's annoying, now these vector names are 1 based, and points are 0 based.  
	//WTF?  Consistency?  Nah.

	double denominator = (inputFace.A * inputRay.direction.x) + (inputFace.B * inputRay.direction.y) + (inputFace.C * inputRay.direction.z);
	//cout << "den:" << denominator << endl;
	double numerator = 0 - ((inputFace.A * inputRay.origin.x) + (inputFace.B * inputRay.origin.y) + (inputFace.C * inputRay.origin.z) + inputFace.D);
	if(thresholdEquals(denominator,0))
	{
		//if the denominator is zero, the ray grazes the plane, never intersects.
		//cout << "Demonimator is 0." << endl;
		return -1;
	}
	else
	{
		double t = numerator / denominator;
		if(thresholdEquals(t,0))
		{
			//cout << "Intersect at t=0" << endl;
			//if t is 0, the ray intersects at the ray's origin.  That doesn't count.
			return -1;
		}
		else if (t < 0)
		{
			//cout << "Negative T" << endl;
			//object is behind the eye.  No intersection.
			return -1;
		}
		else
		{
			//cout << "Plane intersection found.  Will determine if it is a triangle intersection." << endl;
			//We have to check if the intersection point is actually inside the triangle.
			//Calculate barycentric coordinates.
			// The area of an arbitrary triangle can also be
			//computed as half of the length of the cross
			//product of the vectors describing any two of
			//its sides:
			//wait, N is a normal vector.  Already have that.
			PointType intersectionPoint;
			//px = (x0 + t⋅xd)
			//py = (y0 + t⋅yd)
			//pz = (z0 + t⋅zd)
			intersectionPoint.x = inputRay.origin.x + t * inputRay.direction.x;
			intersectionPoint.y = inputRay.origin.y + t * inputRay.direction.y;
			intersectionPoint.z = inputRay.origin.z + t * inputRay.direction.z;

			VectorType n = inputFace.e1.crossProduct(inputFace.e2);//.normalize();
			double totalArea = n.vectorLength() / 2;
			VectorType centerV1 = vectorFromHereToPoint(intersectionPoint, inputFace.v1);
			VectorType centerV2 = vectorFromHereToPoint(intersectionPoint, inputFace.v2);
			VectorType centerV3 = vectorFromHereToPoint(intersectionPoint, inputFace.v3);

			double areaA = centerV1
				.crossProduct(centerV2).vectorLength()/2;
			double areaB = centerV1
				.crossProduct(centerV3).vectorLength()/2;
			double areaC = centerV2
				.crossProduct(centerV3).vectorLength()/2;

			double alpha = areaA / totalArea;
			double beta = areaB / totalArea;
			double gamma = areaC / totalArea;

			//cout <<"sumArea:" << (areaA + areaB + areaC) << endl;
			//cout <<"sumGreek:" << (alpha + beta + gamma) << endl;
			//cout <<"totalArea:" << totalArea << endl;

			//I know that the point is outside the traingle if any of alpha,beta, or gamma 
			//is < 0 or > 1
			if(alpha > 1 || beta > 1 || gamma > 1 || alpha < 0 || beta < 0 || gamma < 0)
			{
				//cout << "outside triangle" << endl;
				return -1;
			}

			//if (alpha + beta + gamma == 1) {
			// point is in triangle
			if(thresholdEquals((alpha+ beta + gamma),1))
			{
				//cout <<"Triangle intersection found." << endl;
				return t;
			}

		}
	}
	//I don't think we should get here.  But if there's a problem, assume no intersect.
	return -1;
}

double Common::clamp(double input)
{
	return (input>1)?1:(input<0)?0:input;
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

vector<string> Common::tokenizeString(string input, string delimiter)
{
	//split a string by a space delimiter.
	//returns a vector of token strings.
	vector<string> tokens;
	int spaceLocation;
	string tokenBuffer = trim(input);
	
	while(true)
	{
		//cout << "tokenizing this:" << tokenBuffer << "\n";
		spaceLocation = tokenBuffer.find(delimiter);
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
			//tokenBuffer = tokenBuffer.substr(spaceLocation + 1, tokenBuffer.length());
			tokenBuffer = tokenBuffer.substr(spaceLocation + delimiter.length(), tokenBuffer.length());
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
		cout << "You should have exactly one argument, the input file name.  No arguments means I'm going to run Demos.\n";
		inputFileName = "";
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

bool Common::thresholdEquals(double a, double b)
{
	//Matt suggested we use a threshold compare, rather than direct equals.
	//This is because floating point values are silly, and represnt numbers in funny ways.
	//This method will return true if (a-b < episolon)
	//which means a and b are close enough to be considered equal.
	//Pick a small epsilon as a threshold
	//I apparenlty arbitrarily chose 10^-6
	double epsilon = .000001;
	double comparison = (a>b)?a-b:b-a;
	//cout << "Attempting a comparison,a,b: " <<comparison<<","<< a << "," << b << endl;
	return (comparison < epsilon);
}