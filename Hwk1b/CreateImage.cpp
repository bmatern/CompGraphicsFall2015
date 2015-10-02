/*
 * Ben Matern
 * Homework 1b
 * Light and Shadow
 * Computer Graphics
 *
 * This program can create a ASCII Format image from an input file.
 * Main method is at the very bottom.
 *
 */

#include <vector>
#include <math.h>
#include "PointType.h"
#include "VectorType.h"
#include "RayType.h"
#include "ColorType.h"
#include "SphereType.h"
#include "ViewingWindowType.h"
#include "Common.h"
using namespace std;

void loadSceneInformation(string inputFileName, RayType& eyeRay, VectorType& upVector, int& width, int& height, double& fovH
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
	    		    	
				vector<string> tokens = Common::tokenizeString(line);
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
						if(eyeRay.direction.vectorLength() == 0)	
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

						if(upVector.vectorLength() == 0)	
						{
							cout << "Up vector has length 0.  Please fix the input file." << endl;
							throw(1);
						}
					
					}
					else if(inputVarName.compare("fovh") == 0)
					{
						fovH = stof(tokens[1]);
						if(upVector.vectorLength() > 180)	
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

void setViewingWindow(ViewingWindowType& viewingWindow, RayType eyeRay, VectorType upVector, int width, int height, double fovH
	, double& d, double& w, double& h , VectorType& u, VectorType& v)
{
	//calculate the viewing window parameters.
	//Mostly we want to find out the u and v vectors, and the 
	//4 corners of the window.

	//the u vector is the unit vector orthogonal to the viewing direction and the up direction.
	// u' is the non-normalized u vector.
	// view_dir x up_dir = u'
	// u = u'/ ||u'||	
	VectorType uPrime = eyeRay.direction.crossProduct(upVector);
	u = uPrime.normalizeVector();
	
	//v is a unit vector orthogonal to both the viewing direction and u.
	VectorType vPrime = u.crossProduct(eyeRay.direction);
	v = vPrime.normalizeVector();
	
	// d is an arbitrary viewing distance, from eye to viewing window.  It is in world coordinates.
	d = 1.0;
	// w is the width of the viewing window.  In 3d world coordinate units.
	// I need to calculate w based on the tangent of fovH and d.  Algebra and trig yield this equation:
	// w = 2 * d * tan (fovH / 2)
	w = 2.0 * d * tan (fovH / 2);
	// calculate h, in world coordinates.  Use the aspect ratio calculated from the width and height in pixels.
	h = w * height / width ;
	
	//n is the normalized view direction vector
	VectorType n = eyeRay.direction.normalizeVector();
	
	//each of these calculations are one point + 3 vectors.
	//This results in a point (well, 4 points)
	//ul = view_origin + d⋅n + h/2⋅v – w/2⋅u
	//ur = view_origin + d⋅n + h/2⋅v + w/2⋅u
	//ll = view_origin + d⋅n – h/2⋅v – w/2⋅u
	//lr = view_origin + d⋅n – h/2⋅v + w/2⋅u
	viewingWindow.topLeft = 
		eyeRay.origin.getVectorFromPoint().addVectors(
		n.multiplyVector(d)).addVectors(
		v.multiplyVector(h/2)).addVectors(
		u.multiplyVector(-w/2)).getPointFromVector();		
	viewingWindow.topRight = 
		eyeRay.origin.getVectorFromPoint().addVectors(
		n.multiplyVector(d)).addVectors(
		v.multiplyVector(h/2)).addVectors(
		u.multiplyVector(w/2)).getPointFromVector();
	viewingWindow.botLeft = 
		eyeRay.origin.getVectorFromPoint().addVectors(
		n.multiplyVector(d)).addVectors(
		v.multiplyVector(-h/2)).addVectors(
		u.multiplyVector(-w/2)).getPointFromVector();		
	viewingWindow.botRight = 
		eyeRay.origin.getVectorFromPoint().addVectors(
		n.multiplyVector(d)).addVectors(
		v.multiplyVector(-h/2)).addVectors(
		u.multiplyVector(w/2)).getPointFromVector();
			
	cout << "Double check the aspect ratios to see that they match." << endl;
	double pixAspRat = 1.0 * width /height;
	cout << "Pixel aspect ratio:" << pixAspRat << endl;
	double worldAspRat = (viewingWindow.botLeft.vectorFromHereToPoint(viewingWindow.botRight).vectorLength())
		/ (viewingWindow.botLeft.vectorFromHereToPoint(viewingWindow.topLeft).vectorLength());
	cout << "World aspect ratio:" << worldAspRat << endl;

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

ColorType phongLighting(ColorType inputColor)
{
	//The basic Phong illumination equation:
	//Iλ = ka Odλ + kd Odλ(N ⋅L)+ ks Osλ (N ⋅H)^n
	//ka, kd, ks are constants for ambient, diffuse, specular weights.  "Reflectivity"
	//Odλ is the color of the object (sphere in this case)
	//n controls the effective rate of falloff in the intensity
	//	of the specular highlight as the direction of reflection differs from
	//	the direction to the viewer
	//	(It's how shiny the specular spot is)
	//Osλ is the color of the specular highlight.  Start with white.  Other stuff will be ridiculous.
	//N = UNIT VECTOR for the direction of hte surface normal to the point where illlumination is evaluated.
	//	should be able to calculate that based on center of hte sphere.  equation on p16 of notes
	//L = UNIT VECTOR from the point evaulated towards the light source.  Easy.  Depends on point light sources etc.
	//H = UNIT vector that represents the direction that is halfway between direction of light and direction to viewer.
	//	equation is on p29
	//Calculate specular and diffuse multiple times for different light sources.



	return inputColor;
}

ColorType trace_ray(ViewingWindowType viewingWindow, RayType eyeRay, int x, int y, VectorType deltaH, VectorType deltaV
	, ColorType bgColor, vector<SphereType> spheres)
{
	//default to background color.  Switch to sphere color if ray intersects it.
	ColorType results = bgColor;	
	//This is the mapping to the world coordinate point.
	//topleft + deltaH*x + deltaV * y
	PointType worldCoordinatesPoint = 
		viewingWindow.topLeft.getVectorFromPoint().addVectors(
		deltaH.multiplyVector(x)).addVectors(
		deltaV.multiplyVector(y)).getPointFromVector();
		
	RayType rayToTrace;
	rayToTrace.origin = eyeRay.origin;
	rayToTrace.direction = rayToTrace.origin.vectorFromHereToPoint(worldCoordinatesPoint).normalizeVector();
			
	//determine intersection points with each sphere.
	double intersectDistance = -1;
	for(int i = 0; i < spheres.size(); i++)
	{
		double currentIntersectDistance = intersectSphere(rayToTrace, spheres[i]);
		//I hope this logic gives me the very closest non-negative sphere distance.  
		if(currentIntersectDistance != -1 && 
			(currentIntersectDistance < intersectDistance || intersectDistance == -1))
		{
			//Here' where I should call the phong model.
			results = phongLighting(spheres[i].color);
			intersectDistance = currentIntersectDistance;
		}	
	}
			
			
	return results;
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
	
	VectorType upVector;
	VectorType u;
	VectorType v;	
	
	ColorType bgColor;
	ColorType currentMaterialColor;
	
	vector<SphereType> spheres;
	
	//The angle of the horizontal field of view.  Theta.
	double fovH;
	
	//contains 4 points for the coordinates of the viewing window.
	ViewingWindowType viewingWindow;

	string inputFileName;

//Read commandline args to get the input file name.
	inputFileName = Common::getInputFileName(argc, argv);
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
	VectorType deltaH = viewingWindow.topLeft.vectorFromHereToPoint(viewingWindow.topRight).multiplyVector(1.0/(width - 1));
	VectorType deltaV = viewingWindow.topLeft.vectorFromHereToPoint(viewingWindow.botLeft).multiplyVector(1.0/(height - 1));

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			ColorType currentPixel = trace_ray(viewingWindow, eyeRay, x, y, deltaH, deltaV, bgColor, spheres);
			pixelArray [ x + y*width ] = currentPixel;
		}
	}
	
//Write the data to a image file.
	Common::writeImageFile(pixelArray, width, height, inputFileName);

	return 0;
}


