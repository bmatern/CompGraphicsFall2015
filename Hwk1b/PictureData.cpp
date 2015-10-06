#include "PictureData.h"

void PictureData::loadSceneInformation()
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
						eyeRay.origin.x = stod(tokens[1]);
						eyeRay.origin.y = stod(tokens[2]);
						eyeRay.origin.z = stod(tokens[3]);			

					}		
					else if(inputVarName.compare("viewdir") == 0)
					{
						eyeRay.direction.x = stod(tokens[1]);
						eyeRay.direction.y = stod(tokens[2]);
						eyeRay.direction.z = stod(tokens[3]);
						if(eyeRay.direction.vectorLength() == 0)	
						{
							cout << "View direction vector has length 0.  Please fix the input file." << endl;
							throw(1);
						}

					}
					else if(inputVarName.compare("updir") == 0)
					{
						upVector.x = stod(tokens[1]);
						upVector.y = stod(tokens[2]);
						upVector.z = stod(tokens[3]);

						if(upVector.vectorLength() == 0)	
						{
							cout << "Up vector has length 0.  Please fix the input file." << endl;
							throw(1);
						}
					
					}
					else if(inputVarName.compare("fovh") == 0)
					{
						fovH = stod(tokens[1]);
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
						bgColor.r = stod(tokens[1]);
						bgColor.g = stod(tokens[2]);
						bgColor.b = stod(tokens[3]);
					}
					else if(inputVarName.compare("mtlcolor") == 0)
					{
						currentMaterial.color.r = stod(tokens[1]);
						currentMaterial.color.g = stod(tokens[2]);
						currentMaterial.color.b = stod(tokens[3]);

						currentMaterial.specularHighlight.r = stod(tokens[4]);
						currentMaterial.specularHighlight.g = stod(tokens[5]);
						currentMaterial.specularHighlight.b = stod(tokens[6]);

						currentMaterial.ka = stod(tokens[7]);
						currentMaterial.kd = stod(tokens[8]);
						currentMaterial.ks = stod(tokens[9]);

						currentMaterial.n = stod(tokens[10]);
					}
					else if(inputVarName.compare("sphere") == 0)
					{
						SphereType currentSphere;
							
						currentSphere.center.x = stod(tokens[1]);
						currentSphere.center.y = stod(tokens[2]);
						currentSphere.center.z = stod(tokens[3]);
						currentSphere.radius = stod(tokens[4]);

						currentSphere.material = currentMaterial;
					
						spheres.push_back(currentSphere);					
					}
					else if(inputVarName.compare("light") == 0)
					{
						cout << "Light source Found" << endl;
						LightType currentLight;

						bool isDirectional = (stoi(tokens[4])==1);		
						currentLight.isDirectional = isDirectional;

						if(isDirectional)	
						{
							currentLight.direction.x = stod(tokens[1]);
							currentLight.direction.y = stod(tokens[2]);
							currentLight.direction.z = stod(tokens[3]);
						}	
						else
						{
							currentLight.center.x = stod(tokens[1]);
							currentLight.center.y = stod(tokens[2]);
							currentLight.center.z = stod(tokens[3]);
						}		

						currentLight.color.r = stod(tokens[5]);	
						currentLight.color.g = stod(tokens[6]);	
						currentLight.color.b = stod(tokens[7]);	
					
						lights.push_back(currentLight);					
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


void PictureData::setViewingWindow()
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

	//deltaH and deltaV are VECTORS.  they indicate how far to increment the viewing window for each pixel.
	//deltaH = (ur - ul)/(pixwidth - 1)
	deltaH = viewingWindow.topLeft.vectorFromHereToPoint(viewingWindow.topRight).multiplyVector(1.0/(width - 1));
	deltaV = viewingWindow.topLeft.vectorFromHereToPoint(viewingWindow.botLeft).multiplyVector(1.0/(height - 1));


	pixelArray =new ColorType[width*height];
}

bool PictureData::isShaded(int x, int y)
{
	return false;
}

ColorType PictureData::shadeRay(int x, int y, SphereType sphere)
{
	//The basic Phong illumination equation:
	//Iλ = ka Odλ + kd Odλ(N ⋅L)+ ks Osλ (N ⋅H)^n
	//ka, kd, ks are constants for ambient, diffuse, specular weights.  "Reflectivity"
	//for multiple light sources, calculate ambient once.  Then diffuse and specular for each light source.
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

	/*if(isShaded(x,y))
	{
		cout << "it's shaded." << endl;
	}
	else
	{
		cout << "it's not shaded." << endl;
	}*/

	//ka Odλ
	ColorType ambientComponent = sphere.material.color.multiplyColor(sphere.material.ka);

	ColorType diffuseComponent;
	for(int i = 0; i < lights.size(); i++)
	{

	}

	return sphere.material.color;
}


ColorType PictureData::traceRay(int x, int y)
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
		double currentIntersectDistance = Common::intersectSphere(rayToTrace, spheres[i]);
		//I hope this logic gives me the very closest non-negative sphere distance.  
		if(currentIntersectDistance != -1 && 
			(currentIntersectDistance < intersectDistance || intersectDistance == -1))
		{
			//Here' where I should call the phong model.
			results = shadeRay(x,y,spheres[i]);


			intersectDistance = currentIntersectDistance;
		}	
	}
			
			
	return results;
}

void PictureData::traceRays()
{
//foreach pixel in image array:
	//call trace_ray() with appropriate parameters
	//Use value returend by trace_ray to update pixel colors.
	cout << "Starting the loop through each pixel" << endl;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixelArray [ x + y*width ] = traceRay(x, y);
		}
	}
}
