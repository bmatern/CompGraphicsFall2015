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
						cout << "Eye Found " << eyeRay.origin.x << "," << eyeRay.origin.y << "," << eyeRay.origin.z << endl;	

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

						cout << "Read a material. rgb=" << currentMaterial.color.r << "," << currentMaterial.color.g << "," << currentMaterial.color.b
							<< " specular = " << currentMaterial.specularHighlight.r << "," << currentMaterial.specularHighlight.g << "," << currentMaterial.specularHighlight.b 
							<< " ka=" << currentMaterial.ka << " kd=" << currentMaterial.kd << " ks=" << currentMaterial.ks << " n=" << currentMaterial.n
							<< endl;
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

						bool isDirectional = (stoi(tokens[4])==0);	
						cout << "isDirectional" << isDirectional << endl;;	
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

						currentLight.intensity.r = stod(tokens[5]);	
						currentLight.intensity.g = stod(tokens[6]);	
						currentLight.intensity.b = stod(tokens[7]);	
					
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

bool PictureData::isShaded(PointType origin, VectorType L, LightType light)
{
	//L is the vector TOWARDS the light source.
	RayType shadowRay;
	shadowRay.origin = origin;
	shadowRay.direction = L;

	for(int i = 0; i < spheres.size(); i++)
	{
		double t = Common::intersectSphere(shadowRay,spheres[i]);
		//t == -1 means it missed.  t==0 means we hit the originating sphere.  Don't count that as an intersection.
		if(t != -1  && !Common::thresholdEquals(t,0))
		{
			//return true;
			//hit!  We intersected a sphere.
			//This point is shaded.
			if(light.isDirectional)
			{
				return true;
			}
			else
			{
				//this is a point light.
				//is the intersection beyond the light source?
				double distanceToLightSource = origin.vectorFromHereToPoint(light.center).vectorLength();
				if(distanceToLightSource > t)
				{
					return true;
				}
				//D on't return anything here cuz the light is closer than the object.
			}
		}
	}
	
	return false;
}

double PictureData::phongLighting(int colorIndex, MaterialType material, VectorType N, VectorType V, PointType intersectPoint)
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
	//Calculate specular and diffuse multiple times for different light sources.  Use the max.

	//colorIndex should be 0,1,2 corresponding with r,g,b
	//think of a better way to do that.

	double colorValue = (colorIndex==0) ? material.color.r 
					: (colorIndex==1) ? material.color.g 
					: material.color.b;

	double specularValue = (colorIndex==0) ? material.specularHighlight.r 
					: (colorIndex==1) ? material.specularHighlight.g 
					: material.specularHighlight.b;


	//ka Odλ
	double ambientComponent = colorValue * material.ka;

	//kd Odλ(N ⋅L)	
	double diffuseComponent;

	//ks Osλ (N ⋅H)^n
	double specularComponent;

	//Get the max diffuse and specular for each light source.
	for(int i = 0; i < lights.size(); i++)
	{
		LightType currentLight = lights[i];
		VectorType L;

		double lightIntensity = (colorIndex==0) ? currentLight.intensity.r 
								: (colorIndex==1) ? currentLight.intensity.g 
								: currentLight.intensity.b;

		if(currentLight.isDirectional)
		{
			L = currentLight.direction.multiplyVector(-1).normalizeVector();
		}
		else
		{
			L = intersectPoint.vectorFromHereToPoint(currentLight.center).normalizeVector();
		}


		bool shadow = isShaded(intersectPoint, L, currentLight);

		//There is no diffuse or specular lighting if it's under a shadow.

		if(!shadow)
		{
			//I*kd Odλ(N ⋅L)
			double currentDiffuse = Common::clamp(
					colorValue 
					* material.kd
					* lightIntensity
					* (N.dotProduct(L)));

			//Find the max diffuse color component.
			diffuseComponent = (currentDiffuse > diffuseComponent) ? currentDiffuse : diffuseComponent;
			//diffuseComponent = Common::clamp(currentDiffuse + diffuseComponent);

			//H = (L + V) / ||L + V||
			VectorType H = L.addVectors(V).multiplyVector(
				1.0/(L.addVectors(V).vectorLength())
				).normalizeVector();
			//H = H.multiplyVector(1.0/H.vectorLength());

			//ks Osλ (N ⋅H)^n
			double currentSpecular = Common::clamp(
					specularValue
					* material.ks
					* lightIntensity
					* (pow(N.dotProduct(H),material.n))
					);

			specularComponent = (currentSpecular > specularComponent) ? currentSpecular : specularComponent;
			//specularComponent = Common::clamp(currentSpecular + specularComponent);
		}
	}

	return ambientComponent
		+ diffuseComponent
		+ specularComponent
		;
}

ColorType PictureData::shadeRay(SphereType sphere, RayType tracedRay, PointType intersectPoint)
{
	ColorType results;

	VectorType N = sphere.center.vectorFromHereToPoint(intersectPoint).normalizeVector();
	//V is the unit vector towards the viewer
	//VectorType V = eyeRay.direction.multiplyVector(-1.0).normalizeVector();
	VectorType V = intersectPoint.vectorFromHereToPoint(eyeRay.origin).normalizeVector();

	results.r = phongLighting(0
		, sphere.material
		, N, V
		, intersectPoint
		);

	results.g = phongLighting(1
		, sphere.material
		, N, V
		, intersectPoint
		);

	results.b = phongLighting(2
		, sphere.material
		, N, V
		, intersectPoint
		);

	return results;
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
	SphereType intersectSphere;
	for(int i = 0; i < spheres.size(); i++)
	{
		double currentIntersectDistance = Common::intersectSphere(rayToTrace, spheres[i]);
		//I hope this logic gives me the very closest non-negative sphere distance.  
		if(currentIntersectDistance != -1 && 
			(currentIntersectDistance < intersectDistance || intersectDistance == -1))
		{
			intersectDistance = currentIntersectDistance;
			intersectSphere = spheres[i];
		}	
	}

	if(intersectDistance != -1)
	{
		//It's hit.
		PointType intersectPoint = rayToTrace.direction.multiplyVector(intersectDistance).addVectors(rayToTrace.origin).getPointFromVector();
		results = shadeRay(intersectSphere, rayToTrace, intersectPoint);

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
