#include "PictureData.h"

void PictureData::createImage(string ifn)
{
	//PictureData pictureConfig;

	inputFileName = ifn;
	//cout << "Input:" << pictureConfig.inputFileName << ":\n";

//Read input file to get the scene information.
	try
	{
		loadSceneInformation();
	}
	catch(int e)
	{
		cout << "Exception occured during loadSceneInformation.  Doublecheck your input file.  Cannot recover." << endl;
		//return(0);
	}

//Perform necessary preliminary calculations.
//Set up the viewing window etc.
	cout << "Starting Preliminary Calculations" << endl;
	setViewingWindow();

//foreach pixel in image array, trace a ray
	cout << "Tracing Rays" <<endl;
	traceRays();
	
//Write the data to a image file.
	cout << "Writing Image File" << endl;
	writeImageFile();
}

void PictureData::writeImageFile()
{
	string widthString = to_string(width);
	string heightString = to_string(height);

	string outputFileName = inputFileName.substr(0,inputFileName.length()-4) + ".ppm";

	//Write image File
	ofstream myfile;
	myfile.open (outputFileName);

	//Write Header
	myfile << "P3\n";
	//myfile << "# This image was created by Ben Matern from input file " << inputFileName << "\n";
	myfile << widthString + " " + heightString + " ";
	myfile << "255\n";

	//Loop through each pixel in the picture.
	for(int y = 0; y < height ; y++)
	{
		for(int x = 0; x < width ; x++)
		{
			string r = Common::getColorValue(pixelArray[ x + y*width ].r);
			string b = Common::getColorValue(pixelArray[ x + y*width ].b);
			string g = Common::getColorValue(pixelArray[ x + y*width ].g);
			myfile << r << g << b;
			//lets try newline after each pixel.
			myfile << "\n";
		}
		//We just finished a row of pixels.
		//myfile << "\n";
	}

	myfile << "";
	myfile.close();
}

void PictureData::loadSceneInformation()
{
	//we'll increment to 0 when the texture is loaded.
	currentTextureIndex = -1;
	isParallel = false;

	//read input file to determine the components of the scene.
	string line = "";
	ifstream myInputfile (inputFileName);
	//cout << "About to open file:" <<inputFileName << endl;
	if (myInputfile.is_open())
	{
		//cout << "Input File Found.  Good.\n";
		while ( getline (myInputfile,line) )
	    {
	    	//cout << "Line:" << line << "\n";
	    	//Check for comments in this line.
	    	if(line.length() > 0 && line.at(0) != '#')
	    	{
	    		    	
				vector<string> tokens = Common::tokenizeString(line, " ");
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
						//cout << "Eye Found " << eyeRay.origin.x << "," << eyeRay.origin.y << "," << eyeRay.origin.z << endl;	

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

						currentMaterial.opacity = stod(tokens[11]);
						currentMaterial.indexOfRefraction = stod(tokens[12]);

						//cout << "opacity:" << currentMaterial.opacity << endl;
						//cout << "indexOfRefraction:" << currentMaterial.indexOfRefraction << endl;

						//currentMaterial.hasTexture = false;
						//texture index of -1 means it's a solid color.
						currentMaterial.textureIndex = -1;

						//materials.push_back(currentMaterial);
						//currentMaterialIndex++;

						/*cout << "Read a material. rgb=" << currentMaterial.color.r << "," << currentMaterial.color.g << "," << currentMaterial.color.b
							<< " specular = " << currentMaterial.specularHighlight.r << "," << currentMaterial.specularHighlight.g << "," << currentMaterial.specularHighlight.b 
							<< " ka=" << currentMaterial.ka << " kd=" << currentMaterial.kd << " ks=" << currentMaterial.ks << " n=" << currentMaterial.n
							<< endl;*/
					}
					else if(inputVarName.compare("texture") == 0)
					{
						//currentMaterial.hasTexture = true;		
						//Program seems to run super slow when it has textures.
						TextureType currentTexture = Common::loadTexture(tokens[1]);
						textures.push_back(currentTexture);
						currentTextureIndex++;

						//currentMaterial.texture = Common::loadTexture(tokens[1]);
						currentMaterial.textureIndex = currentTextureIndex;
						//materials.push_back(currentMaterial);
						
						//currentMaterial.loadTexture();
						//cout<<"Texture file name detected: " << tokens[1] << endl;		
					}
					else if(inputVarName.compare("sphere") == 0)
					{
						SphereType currentSphere;
							
						currentSphere.center.x = stod(tokens[1]);
						currentSphere.center.y = stod(tokens[2]);
						currentSphere.center.z = stod(tokens[3]);
						currentSphere.radius = stod(tokens[4]);

						currentSphere.material = currentMaterial;
						//currentSphere.materialIndex = currentMaterialIndex;
					
						spheres.push_back(currentSphere);					
					}
					else if(inputVarName.compare("light") == 0)
					{
						//cout << "Light source Found" << endl;
						LightType currentLight;

						bool isDirectional = (stoi(tokens[4])==0);	
						//cout << "isDirectional" << isDirectional << endl;;	
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
					else if(inputVarName.compare("v") == 0)
					{
						//cout << "v = vertex detected" << endl;
						PointType vertex;
						vertex.x = stod(tokens[1]);
						vertex.y = stod(tokens[2]);
						vertex.z = stod(tokens[3]);
						vertices.push_back(vertex);

					}
					else if(inputVarName.compare("vt") == 0)
					{
						//cout << "vt = vertex texture detected" << endl;
						TextureCoordinateType tc;
						tc.u = stod(tokens[1]);
						tc.v = stod(tokens[2]);
						textureCoordinates.push_back(tc);
					}
					else if(inputVarName.compare("vn") == 0)
					{
						//cout << "vt = vertex normal detected" << endl;
						VectorType vn;
						vn.x = stod(tokens[1]);
						vn.y = stod(tokens[2]);
						vn.z = stod(tokens[3]);
						vn = vn.normalizeVector();
						vertexNormals.push_back(vn);
					}
					else if(inputVarName.compare("f") == 0)
					{
						//cout << "f = face detected" << endl;
						FaceType face;
						//faces look like this
						//f v1/vt1/vn1 v2/vt2/vn2 v3/vt2/vn3
						//• And for a smooth-shaded, untextured triangle:
						//f v1//vn1 v2//vn2 v3//vn3
						//They're 1 based indices into the arrays i created.
						//I need to subtract 1 to make them 0 based.
						//And store the values from the arrays.
						//I should probably store an index into the arrays.
						//instead of copying the data
						//But I don't want to right now.

						
						//cout << "tokens1:" << tokens[1] << endl;
						//cout << "tokens2:" << tokens[2] << endl;
						//cout << "tokens3:" << tokens[3] << endl;

						face.material = currentMaterial;

						int v1,v2,v3,
							vt1,vt2,vt3,
							vn1,vn2,vn3;

						vector<string> faceTokens1, faceTokens2, faceTokens3;

						faceTokens1 = Common::tokenizeString(tokens[1],"//");
						if(faceTokens1.size() == 2)
						{
							//cout << "About to tokenize:" << tokens[2] << endl;
							faceTokens2 = Common::tokenizeString(tokens[2],"//");
							//cout << "faceTokens2Size:" << faceTokens2.size() << endl;
							//cout << "About to tokenize:" << tokens[3] << endl;
							faceTokens3 = Common::tokenizeString(tokens[3],"//");
						}
						else
						{			
							faceTokens1 = Common::tokenizeString(tokens[1],"/");
							faceTokens2 = Common::tokenizeString(tokens[2],"/");
							faceTokens3 = Common::tokenizeString(tokens[3],"/");

						}
						//cout << "faceTokens1Size:" << faceTokens1.size() << endl;
						//cout << "faceTokens2Size:" << faceTokens2.size() << endl;
						//cout << "faceTokens3Size:" << faceTokens3.size() << endl;


						//vector<string> faceTokens1 = Common::tokenizeString(tokens[1],"/");
						//vector<string> faceTokens2 = Common::tokenizeString(tokens[2],"/");
						//vector<string> faceTokens3 = Common::tokenizeString(tokens[3],"/");

						v1 = stoi(faceTokens1[0]) - 1;
						v2 = stoi(faceTokens2[0]) - 1;
						v3 = stoi(faceTokens3[0]) - 1;


						face.v1 = vertices[v1];
						face.v2 = vertices[v2];
						face.v3 = vertices[v3];

						//Sometimes there aren't texture coordinates, if there is no texture
						//cout << "faceTokens1:" << faceTokens1[1] << endl;
						if(faceTokens1.size() == 3)
						{
							//cout << "tokens size 3. " << endl;
							vt1 = stoi(faceTokens1[1]) - 1;
							vt2 = stoi(faceTokens2[1]) - 1;
							vt3 = stoi(faceTokens3[1]) - 1;
							
							vn1 = stoi(faceTokens1[2]) - 1;
							vn2 = stoi(faceTokens2[2]) - 1;
							vn3 = stoi(faceTokens3[2]) - 1;

							face.vt1 = textureCoordinates[vt1];
							face.vt2 = textureCoordinates[vt2];
							face.vt3 = textureCoordinates[vt3];

							face.vn1 = vertexNormals[vn1];
							face.vn2 = vertexNormals[vn2];
							face.vn3 = vertexNormals[vn3];
						}
						else
						{
							//cout << "tokens size 2. " << endl;
							//only 2 tokens, we don't have texture coordinates here.
							//vt1 = stoi(faceTokens1[1]) - 1;
							//vt2 = stoi(faceTokens2[1]) - 1;
							//vt3 = stoi(faceTokens3[1]) - 1;
							//cout << "facetokens1:" << faceTokens1[1] << endl;
							
							vn1 = stoi(faceTokens1[1]) - 1;
							vn2 = stoi(faceTokens2[1]) - 1;
							vn3 = stoi(faceTokens3[1]) - 1;
							//cout <<"vns:" << vn1 << "," << vn2 << "," << vn3 ;

							//face.vt1 = textureCoordinates[vt1];
							//face.vt2 = textureCoordinates[vt2];
							//face.vt3 = textureCoordinates[vt3];

							face.vn1 = vertexNormals[vn1];
							face.vn2 = vertexNormals[vn2];
							face.vn3 = vertexNormals[vn3];
						}


						face.e1 = Common::vectorFromHereToPoint(face.v1, face.v2);
						face.e2 = Common::vectorFromHereToPoint(face.v1, face.v3);
						//do i need to normalize?
						//No, shouldn't be necessary.
						VectorType n = face.e1.crossProduct(face.e2);//.normalize();
						face.A = n.x;
						face.B = n.y;
						face.C = n.z;
						//Ax + By + Cz + D = 0
						//Use any point to solve for D.
						face.D = 0 - (face.A * face.v1.x) - (face.B * face.v1.y) - (face.C * face.v1.z);
						//cout << "A,B,C,D:" << A << "," << B << "," << C << "," << D << endl;
						//cout << "Input Ray xyz:" << inputRay.direction.x << "," << inputRay.direction.y << "," << inputRay.direction.z << endl;

						//Solve for t, where t is the place where the triangle is intersected
						//t = –(A⋅x0 + B⋅y0 + C⋅z0 + D)/(A⋅xd+ B⋅yd+ C⋅zd)

						faces.push_back(face);

						//cout<<"Number tokens should be 3:" << faceTokens1.size() << endl;
					}
					else if(inputVarName.compare("parallel") == 0)
					{
						isParallel = true;
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

	//cout << "u:" << u.x << "," << u.y << "," << u.z << endl;

	
	//v is a unit vector orthogonal to both the viewing direction and u.
	VectorType vPrime = u.crossProduct(eyeRay.direction);
	v = vPrime.normalizeVector();

	//cout << "v:" << v.x << "," << v.y << "," << v.z << endl;
	
	// d is an arbitrary viewing distance, from eye to viewing window.  It is in world coordinates.
	if(isParallel)
	{
		d = 0;
	}
	else
	{
		d = 1.0;
	}
	
	// w is the width of the viewing window.  In 3d world coordinate units.
	// I need to calculate w based on the tangent of fovH and d.  Algebra and trig yield this equation:
	// w = 2 * d * tan (fovH / 2)
	if(isParallel)
	{
		//since distance is 0 here, we need to pick a viewing window width.
		//Can't really calculate it based on what I have.  
		w = 20;
	}
	else
	{
		//w = 2.0 * d * tan ((M_PI/180)*fovH / 2);
		w = 2.0 * d * tan ((M_PI/180)*fovH / 2);
	}
	//cout <<"w:" << w << endl;
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

	//The d portion should be 0 if this is parallel projection.
	/*if(isParallel)
	{
		viewingWindow.topLeft = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			//n.multiplyVector(d)).addVectors(
			v.multiplyVector(h/2)).addVectors(
			u.multiplyVector(-w/2)));		
		viewingWindow.topRight = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			//n.multiplyVector(d)).addVectors(
			v.multiplyVector(h/2)).addVectors(
			u.multiplyVector(w/2)));
		viewingWindow.botLeft = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			//n.multiplyVector(d)).addVectors(
			v.multiplyVector(-h/2)).addVectors(
			u.multiplyVector(-w/2)));		
		viewingWindow.botRight = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			//n.multiplyVector(d)).addVectors(
			v.multiplyVector(-h/2)).addVectors(
			u.multiplyVector(w/2)));
	}
	else*/
	{
		viewingWindow.topLeft = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			n.multiplyVector(d)).addVectors(
			v.multiplyVector(h/2)).addVectors(
			u.multiplyVector(-w/2)));		
		viewingWindow.topRight = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			n.multiplyVector(d)).addVectors(
			v.multiplyVector(h/2)).addVectors(
			u.multiplyVector(w/2)));
		viewingWindow.botLeft = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			n.multiplyVector(d)).addVectors(
			v.multiplyVector(-h/2)).addVectors(
			u.multiplyVector(-w/2)));		
		viewingWindow.botRight = 
			Common::getPointFromVector(Common::getVectorFromPoint(eyeRay.origin).addVectors(
			n.multiplyVector(d)).addVectors(
			v.multiplyVector(-h/2)).addVectors(
			u.multiplyVector(w/2)));
	}

	//cout << "topLeft:" << viewingWindow.topLeft.x << "," << viewingWindow.topLeft.y << "," << viewingWindow.topLeft.z << endl;
	//cout << "topRight:" << viewingWindow.topRight.x << "," << viewingWindow.topRight.y << "," << viewingWindow.topRight.z << endl;
	//cout << "botLeft:" << viewingWindow.botLeft.x << "," << viewingWindow.botLeft.y << "," << viewingWindow.botLeft.z << endl;
	//cout << "botRight:" << viewingWindow.botRight.x << "," << viewingWindow.botRight.y << "," << viewingWindow.botRight.z << endl;
			
	/*cout << "Double check the aspect ratios to see that they match." << endl;
	double pixAspRat = 1.0 * width /height;
	cout << "Pixel aspect ratio:" << pixAspRat << endl;
	double worldAspRat = (viewingWindow.botLeft.vectorFromHereToPoint(viewingWindow.botRight).vectorLength())
		/ (viewingWindow.botLeft.vectorFromHereToPoint(viewingWindow.topLeft).vectorLength());
	cout << "World aspect ratio:" << worldAspRat << endl;*/

	//deltaH and deltaV are VECTORS.  they indicate how far to increment the viewing window for each pixel.
	//deltaH = (ur - ul)/(pixwidth - 1)
	deltaH = Common::vectorFromHereToPoint(viewingWindow.topLeft,viewingWindow.topRight).multiplyVector(1.0/(width - 1));
	deltaV = Common::vectorFromHereToPoint(viewingWindow.topLeft,viewingWindow.botLeft).multiplyVector(1.0/(height - 1));

	//cout << "deltaH:" << deltaH.x << ","  << deltaH.y << ","  << deltaH.z << endl;
	//cout << "deltaV:" << deltaV.x << ","  << deltaV.y << ","  << deltaV.z << endl;


	pixelArray = vector<ColorType>(width*height);
}

ColorType PictureData::traceRay(RayType rayToTrace, int recursionDepth)
{
	//cout <<"Starting traceRay" << x << "," << y << endl;
	//default to background color.  Switch to sphere color if ray intersects it.
	ColorType results = bgColor;	
	//This is the mapping to the world coordinate point.
	//topleft + deltaH*x + deltaV * y


			//cout << "rayToTrace" << rayToTrace.origin.x << "," << rayToTrace.origin.y << "," << rayToTrace.origin.z << ";" <<
			//rayToTrace.direction.x << "," << rayToTrace.direction.y << "," << rayToTrace.direction.z << endl;

	//determine intersection points with each sphere.
	double intersectSphereDistance = -1;
	int intersectSphereIndex = -1;
	//SphereType intersectSphere;
	//cout << "NumSpheres" << spheres.size() << endl;
	for(int i = 0; i < spheres.size(); i++)
	{
		//cout << "ABOUT TO CALL INTERSECT SPHERE:" << endl;
		double currentIntersectDistance = Common::intersectSphere(rayToTrace, spheres[i]);
		//cout  << "currentIntersectDistance:" << currentIntersectDistance << endl;

		//I hope this logic gives me the very closest non-negative sphere distance.  
		if(currentIntersectDistance != -1 && 
			(currentIntersectDistance < intersectSphereDistance || intersectSphereDistance == -1))
		{
			intersectSphereDistance = currentIntersectDistance;
			intersectSphereIndex = i;
			//intersectSphere = spheres[i];
			//cout << "sphereDistance:" << intersectSphereDistance << endl;
		}	
	}

	//determine closest face interaction
	double intersectFaceDistance = -1;
	int intersectFaceIndex = -1;

	for(int i = 0; i < faces.size(); i++)
	{
		double currentIntersectDistance = Common::intersectFace(rayToTrace, faces[i]);
		//I hope this logic gives me the very closest non-negative face distance.  
		if(currentIntersectDistance != -1 && 
			(currentIntersectDistance < intersectFaceDistance || intersectFaceDistance == -1))
		{
			intersectFaceDistance = currentIntersectDistance;
			intersectFaceIndex = i;
			//intersectSphere = spheres[i];
		}	
	}

	/*if(intersectSphereDistance < 0)
	{
		cout << "sphereDistance:" << intersectSphereDistance << endl;
	}*/

	//I'm gonna use this int here to decide if we hit a sphere or face.
	//probably should use a enum or something.
	//0 means no interaction.  1 means a sphere.  2 means intersected a face first.
	int intersectionID= 0;
	//this is kind of a clusterfrick.  I should clean this up.
	if(intersectSphereDistance != -1)
	{
		//We intersected a sphere.  Maybe a face too.
		if(intersectFaceDistance != -1)
		{
			//we intersected a face too.
			if(intersectFaceDistance < intersectSphereDistance)
			{
				intersectionID = 2;
				//Face is closer.  Render the face.
				//We hit a face, not a sphere.
			}
			else
			{
				intersectionID = 1;
				//we hit a sphere, but not a face.
			}
		}
		else
		{
			intersectionID = 1;
			//We just intersected a sphere.  Render hte sphere.
			//we hit a sphere, not a face.
		}
	}
	else if(intersectFaceDistance != -1)
	{
		intersectionID = 2;
		//We interected a face, not a sphere.
	}
	else
	{
		//NOTHING.
	}

	if(intersectionID == 1)
	{
		//intersect a sphere.
		SphereType sphere = spheres[intersectSphereIndex];

		PointType intersectPoint = Common::getPointFromVector(
			rayToTrace.direction.multiplyVector(intersectSphereDistance)
			.addVectors(Common::getVectorFromPoint(rayToTrace.origin)));

		VectorType N = Common::vectorFromHereToPoint(sphere.center,intersectPoint).normalizeVector();
		//V is the unit vector towards the viewer
		//VectorType V = eyeRay.direction.multiplyVector(-1.0).normalizeVector();
		VectorType V = Common::vectorFromHereToPoint(intersectPoint,eyeRay.origin).normalizeVector();
		sphere.material.color = assignTextureColor(sphere, intersectPoint,N);

		results = shadeRay(spheres[intersectSphereIndex].material, rayToTrace, intersectPoint,N,V,recursionDepth);
	}
	else if(intersectionID == 2)
	{
		FaceType face = faces[intersectFaceIndex];
		//Intersect a face.
		PointType intersectPoint = Common::getPointFromVector(
			rayToTrace.direction.multiplyVector(intersectFaceDistance)
			.addVectors(Common::getVectorFromPoint(rayToTrace.origin)));

		VectorType N = calculateN(face, intersectPoint);
	
		//V is the unit vector towards the viewer
			//VectorType V = eyeRay.direction.multiplyVector(-1.0).normalizeVector();
		VectorType V = Common::vectorFromHereToPoint(intersectPoint,eyeRay.origin).normalizeVector();

		//Assign it's color.
		//This method will return the flat color if it doesn't have a texture.
		face.material.color = assignTextureColor(face, intersectPoint,N);

		results = shadeRay(face.material, rayToTrace, intersectPoint,N,V,recursionDepth);
	}
	else 
	{
		//cout << "no intersections at all, returning bacground color" << endl;
	}

	return results;
}

bool PictureData::isShaded(PointType origin, VectorType L, LightType light)
{

	//L is the vector TOWARDS the light source.
	RayType shadowRay;
	shadowRay.origin = origin;
	shadowRay.direction = L;

	for(int i = 0; i < spheres.size(); i++)
	{
		//cout << "Calling intersectSphere from isShaded" << endl;
		double t = Common::intersectSphere(shadowRay,spheres[i]);
		//cout << "Done from isShaded" << endl;
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
				double distanceToLightSource = Common::vectorFromHereToPoint(origin,light.center).vectorLength();
				if(distanceToLightSource > t)
				{
					return true;
				}
				//D on't return anything here cuz the light is closer than the object.
			}
		}
	}

	for(int i = 0; i < faces.size(); i++)
	{
		//cout << "Calling intersectSphere from isShaded" << endl;
		double t = Common::intersectFace(shadowRay,faces[i]);
		//cout << "Done from isShaded" << endl;
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
				double distanceToLightSource = Common::vectorFromHereToPoint(origin,light.center).vectorLength();
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

double PictureData::phongLighting(int colorIndex, MaterialType material,  VectorType N, VectorType V, PointType intersectPoint, ColorType reflectedColor, ColorType transmittedColor)
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
	//The V vector passed in here should be the Incidence vector, from camera to the object intersection.
	//I would be the opposite of that.

	//colorIndex should be 0,1,2 corresponding with r,g,b
	//think of a better way to do that.

	//V = V.normalizeVector();
	//cout << "N:" << N.x << "," << N.y << ","<< N.z << endl;
	N = N.normalizeVector();
	//cout << "N:" << N.x << "," << N.y << ","<< N.z << endl;
	

	double colorValue = (colorIndex==0) ? material.color.r 
					: (colorIndex==1) ? material.color.g 
					: material.color.b;

	double specularValue = (colorIndex==0) ? material.specularHighlight.r 
					: (colorIndex==1) ? material.specularHighlight.g 
					: material.specularHighlight.b;


	//ka Odλ
	double ambientComponent = colorValue * material.ka;

	//kd Odλ(N ⋅L)	
	double diffuseComponent = 0;

	//ks Osλ (N ⋅H)^n
	double specularComponent = 0;

	double reflectedComponent = 0;

	double transmittedComponent = 0;

	//I'm calculating reflectivity based on the simplified Schlick equation.
	//F0 = ((n1-n2)/(n1+n2))^2
	//The n are indices of refraction.
	//air is 1.

	double n1 = 1;
	double n2 = material.indexOfRefraction;
	//shlickRefCof = the reflectance of light when N = V
	double schlickRefCoef = pow( (n1-n2)/(n1+n2) ,2);
	//reflectivity is R(theta) = R0 + (1 - R0)(1 - cos(theta))^5
	//cos theta = N dot V
	//Wait maybe cos theta = I dot N
	//I should be the opposite of V I think.

	VectorType I = V.multiplyVector(-1).normalizeVector();

	double reflectivity = schlickRefCoef + 
		(1 - schlickRefCoef) *
		pow( ( 1 - N.dotProduct(V) ) ,5);
		//pow( ( 1 - I.dotProduct(N) ) ,5);

	//DEBUG.  This isn't right.
	//reflectivity = material.ks;

	reflectedComponent = 
		Common::clamp(
				(colorIndex==0) ? reflectivity * reflectedColor.r 
					: (colorIndex==1) ? reflectivity * reflectedColor.g 
					: reflectivity * reflectedColor.b
		)
		;

	//transmitted Light should look like this.
	//(1− Fr )⋅(e^−αλt)⋅Tλ
	//t is the distance traveled....
	//I gotta calculate that i guess?
	//No wait, don't use that, use this instead:
	//(1− Fr )⋅(1−α)⋅Tλ


	/*double t = 30;
	double transmittedLightCoefficient = 
	(1-reflectivity) *
	(exp( material.opacity * (-1) * (t)))
	;*/

	double transmittedLightCoefficient = 
	(1-reflectivity) *
	(1-material.opacity)
	;
	

	transmittedComponent = 
		Common::clamp(
				(colorIndex==0) ? transmittedLightCoefficient * transmittedColor.r 
					: (colorIndex==1) ? transmittedLightCoefficient * transmittedColor.g 
					: transmittedLightCoefficient * transmittedColor.b
		)
		;

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
			L = Common::vectorFromHereToPoint(intersectPoint,currentLight.center).normalizeVector();
		}

		
		//This is debugging.  Searching for overflow and underlfow in phong.
		double NdotL = N.dotProduct(L);
		if(NdotL<0)
		{
			//cout << "N dot L is less than 0:" << NdotL << endl;
			NdotL = 0;
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
					* (NdotL));

			//For debugging.....
			//currentDiffuse = 0;

			//Find the max diffuse color component.
			diffuseComponent = (currentDiffuse > diffuseComponent) ? currentDiffuse : diffuseComponent;
			//diffuseComponent = Common::clamp(currentDiffuse + diffuseComponent);

			//H = (L + V) / ||L + V||
			VectorType H = L.addVectors(V).multiplyVector(
				1.0/(L.addVectors(V).vectorLength())
				).normalizeVector();
			//H = H.multiplyVector(1.0/H.vectorLength());

			//Debug a litlte here.  detecting underflow if N . H is less than 0.
			double NdotH = N.dotProduct(H);

			if(NdotH < 0)
			{
				//cout << "N dot H is less than 0:" << NdotH << endl;
				NdotH = 0;
			}

			//ks Osλ (N ⋅H)^n
			double currentSpecular = Common::clamp(
					specularValue
					* material.ks
					* lightIntensity
					* (pow(NdotH,material.n))
					);

			//Set specular to 0 for debugging:
			//currentSpecular = 0;

			specularComponent = (currentSpecular > specularComponent) ? currentSpecular : specularComponent;
			//specularComponent = Common::clamp(currentSpecular + specularComponent);
		}
	}

	return Common::clamp(
		ambientComponent
		+ diffuseComponent
		+ specularComponent
		+ reflectedComponent
		+ transmittedComponent
		)
		;
}

VectorType PictureData::calculateN(FaceType face, PointType intersectPoint)
{
	VectorType nPrime = face.e1.crossProduct(face.e2);//.normalize();
	double totalArea = nPrime.vectorLength() / 2;
	VectorType centerV1 = Common::vectorFromHereToPoint(intersectPoint, face.v1);
	VectorType centerV2 = Common::vectorFromHereToPoint(intersectPoint, face.v2);
	VectorType centerV3 = Common::vectorFromHereToPoint(intersectPoint, face.v3);
	double areaA = centerV1
		.crossProduct(centerV2).vectorLength()/2;
	double areaB = centerV1
		.crossProduct(centerV3).vectorLength()/2;
	double areaC = centerV2
		.crossProduct(centerV3).vectorLength()/2;

	//the alpha/beta/gamma are areas, they should coorespond with the point opposite it 
	//in the triangle.
	//alpha : v1 and v2, mult by v3
	//beta : v1 and v3, mult by v2
	//gamma : v2 and v3, mult by v1.

	double alpha = areaA / totalArea;
	double beta = areaB / totalArea;
	double gamma = areaC / totalArea;


	//n = (α*n0+β*n1+γ*n2) / ||α*n0+β*n1+γ*n2||
	VectorType N = (face.vn1.multiplyVector(gamma) 
		.addVectors(face.vn2.multiplyVector(beta))
		.addVectors(face.vn3.multiplyVector(alpha))
		).normalizeVector();

	return N;
}



ColorType PictureData::shadeRay(MaterialType material, RayType tracedRay, PointType intersectPoint, VectorType N, VectorType V, int recursionDepth)
{
	ColorType results = bgColor;
	int maxRecursionDepth = 3;

	//VectorType N = Common::vectorFromHereToPoint(sphere.center,intersectPoint).normalizeVector();
	//V is the unit vector towards the viewer
	//VectorType V = eyeRay.direction.multiplyVector(-1.0).normalizeVector();
	//VectorType V = Common::vectorFromHereToPoint(intersectPoint,eyeRay.origin).normalizeVector();


	// Calculate Reflected Light.
	// Fr = F0 + (1 – F0)(1 – cosθi)^5 
	// cosθi = I ⋅ N 
	// F0 is the reflectivity when I = N
	// I = Incident Ray.  It's opposite of incoming ray.  I normali
	// R = Reflected Ray
	// R = A + S
	// N = Normal
	// I + S = A

	ColorType reflectedLight = bgColor;
	reflectedLight.r = 0;
	reflectedLight.g = 0;
	reflectedLight.b = 0;

	ColorType transmittedLight = bgColor;
	transmittedLight.r = 0;
	transmittedLight.g = 0;
	transmittedLight.b = 0;


	VectorType I = tracedRay.direction.multiplyVector(-1).normalizeVector();
	//N = N.normalizeVector();
	//cout << flush ;

	//CALCULATE THE REFLECTED RAY
	if(recursionDepth <= maxRecursionDepth)
	{
		//double reflectivity = material.ks;
		//VectorType I, A, S;
		//VectorType I = tracedRay.direction.multiplyVector(-1).normalizeVector();

		//N = N.normalizeVector();
		double a = N.dotProduct(I);

		//R = A + S = (a* N) + (a*N - I) 
		// = 2(aN) - I
		VectorType R = N.multiplyVector(a).multiplyVector(2)
			.addVectors(I.multiplyVector(-1));//.normalize();

		RayType recurseRayToTrace;
		recurseRayToTrace.origin = intersectPoint;
		recurseRayToTrace.direction = R;
		recurseRayToTrace.insideObject = false;

		reflectedLight = traceRay(recurseRayToTrace, recursionDepth + 1);
		//reflectedLight = bgColor;

		//double thetaI = I.angleBetweenVectors(N);
		//S = (I . N) * (N) - I
		//S = (I.dotProduct(N));
		//double Fr = reflectivity + (1 = reflectivity) * (pow(1-cos(),5));

	}

	//CALCULATE THE TRANSMITTED RAY.
	//Don't transmit the ray if alpha is 1.
	//Because that means it's totally opaque.
	if(recursionDepth <= maxRecursionDepth && !Common::thresholdEquals(material.opacity,1))
	{
		//Calculate thetaI, angle of incidence

		//Snells law
		// (sin θi / sin θt) = (ηtλ / ηiλ)

		//index of refractions.
		//"Traced Ray" is the ray that is intersecting the object.  
		//If it's in air, ni is 1.
		//If it's in an object, ni is the objects index of refaction.
		//i = incidence, t = transmitted.
		double ni = tracedRay.insideObject ? tracedRay.indexOfRefraction : 1;
		double nt = tracedRay.insideObject ? 1 : tracedRay.indexOfRefraction;
		
		double NdotI = N.dotProduct(I);
		if(NdotI < 0)
		{
			N = N.multiplyVector(-1).normalizeVector();
			NdotI = N.dotProduct(I);
		}

		double thetaI = acos(N.dotProduct(I));

		//T = (−N) sqrt ( 1− (ηi / ηt)^2 * (1− cos^2(θi))) + (ηi / ηt) * (cos(θi) * N - I )


		VectorType T = (N.multiplyVector(-1)) 
			.multiplyVector( sqrt(
				1 - 
				pow( ni/nt , 2 ) *
				( 1 - pow( cos(thetaI) , 2 ) )
			))
			.addVectors(
				N.multiplyVector(cos(thetaI))
					.addVectors(I.multiplyVector(-1))
						.multiplyVector(ni / nt)
			);

		RayType recurseRayToTrace;
		recurseRayToTrace.origin = intersectPoint;
		recurseRayToTrace.direction = T;
		recurseRayToTrace.insideObject = !tracedRay.insideObject;

		transmittedLight = traceRay(recurseRayToTrace, recursionDepth + 1);
			

	}

	//cout << flush ;

	results.r = phongLighting(0
		, material
		//, materials[sphere.materialIndex]
		//, sphere.materialIndex
		, N, V
		, intersectPoint
		, reflectedLight
		, transmittedLight
		);

	results.g = phongLighting(1
		, material
		//, materials[sphere.materialIndex]
		//, sphere.materialIndex
		, N, V
		, intersectPoint
		, reflectedLight
		, transmittedLight
		);

	results.b = phongLighting(2
		, material
		//, materials[sphere.materialIndex]
		//, sphere.materialIndex
		, N, V
		, intersectPoint
		, reflectedLight
		, transmittedLight
		);

	return results;
}

ColorType PictureData::assignTextureColor(SphereType inputSphere, PointType intersectPoint, VectorType N)
{
	//cout << "Assigning Texture Color to a sphere." << endl;
	if(inputSphere.material.textureIndex == -1)
	{
		//No texture.
		//return flat color
		//cout << "Flat Color" << endl;
		return inputSphere.material.color;
	}
	else
	{
		//cout << "Texture Color" << endl;
		//Texture Map
		//Determine sphere coordinates from the coordinates of intersection


		//φ = acos[(z – zc)/r] = acos(Nz)
		//θ = atan[(y – yc) / (x – xc)] = atan[Ny / Nx]
		// – or – 
		//θ = atan2(Ny, Nx)
		double phi, theta, u, v;

		//phi = acos((intersectPoint.z - inputSphere.center.z) / inputSphere.radius);
		//theta = atan((intersectPoint.y - inputSphere.center.y) / (intersectPoint.x - inputSphere.center.x));
		phi = acos(N.z);
		theta = atan2(N.y,N.x);

		//acos is in range [0,pi], we need to map it to [0,1]
		v = phi / M_PI;

		//theta should be in the range [-pi,pi]
		if(theta < 0)
		{
			theta = theta + (2 * M_PI);
		}
		//now theta is in the range [0,2pi]
		u = theta / (2 * M_PI);
		//u is in the range [0,1]

		if(v < 0.0 || v > 1.0 || u < 0.0 || u > 1.0)
		{
			cout << "Something's not right, we want u and v to be in the range [0,1]." << endl;
		}
		else 
		{
		//	cout << "good.";
		}

		int textureDimension = textures[inputSphere.material.textureIndex].dimension;
		int x = floor(u * textureDimension);
		int y = floor(v * textureDimension);
		//cout << "length n" << N.vectorLength() << endl;
		//cout << "dim,phi,theta:" << textureDimension << "," << phi << "," << theta << endl;
		//cout << x ;
		//cout << y ;
		//cout << textureDimension ;
		//cout << "dim,x,y:" << textureDimension << "," << x  << "," << y << endl;
		//cout << x  << "," << y << endl;
		//for some reason it wants this command, otherwise there is disruption.
		//I really cannot explain it.
		//cout.flush();
		//cout << endl;
		cout << flush ;
		//cout << "." ;
		//x = x;
		//y = y;

		int vectorIndex = y + textureDimension * x;
		return textures[inputSphere.material.textureIndex].pixelArray[vectorIndex];
		//return inputSphere.material.color;

	}
}

ColorType PictureData::assignTextureColor(FaceType face, PointType intersectPoint, VectorType N)
{
	VectorType nPrime = face.e1.crossProduct(face.e2);//.normalize();
	double totalArea = nPrime.vectorLength() / 2;
	VectorType centerV1 = Common::vectorFromHereToPoint(intersectPoint, face.v1);
	VectorType centerV2 = Common::vectorFromHereToPoint(intersectPoint, face.v2);
	VectorType centerV3 = Common::vectorFromHereToPoint(intersectPoint, face.v3);
	double areaA = centerV1
		.crossProduct(centerV2).vectorLength()/2;
	double areaB = centerV1
		.crossProduct(centerV3).vectorLength()/2;
	double areaC = centerV2
		.crossProduct(centerV3).vectorLength()/2;

	//the alpha/beta/gamma are areas, they should coorespond with the point opposite it 
	//in the triangle.
	//alpha : v1 and v2, mult by v3
	//beta : v1 and v3, mult by v2
	//gamma : v2 and v3, mult by v1.

	double alpha = areaA / totalArea;
	double beta = areaB / totalArea;
	double gamma = areaC / totalArea;
	//cout << "Assigning Texture Color" << endl;
	//cout << flush ;
	if(face.material.textureIndex == -1)
	{
		//No texture.
		//return flat color
		return face.material.color;
	}
	else
	{
		//u = α*u0+β*u1+γ*u2
		//v = α*v0+β*v1+γ*v2
		//double u = (alpha * inputFace.vt1.u) + (beta * inputFace.vt2.u) + (gamma * inputFace.vt3.u);
		//I think this one is right.
		double u = (alpha * face.vt3.u) + (beta * face.vt2.u) + (gamma * face.vt1.u);
		double v = (alpha * face.vt3.v) + (beta * face.vt2.v) + (gamma * face.vt1.v);


		if(v < 0.0 || v > 1.0 || u < 0.0 || u > 1.0)
		{
			cout << "Something's not right, we want u and v to be in the range [0,1]." << endl;
		}
		else 
		{
			//	cout << "good.";
		}

		int textureDimension = textures[face.material.textureIndex].dimension;
		int x = floor(u * textureDimension);
		int y = floor(v * textureDimension);
		

		int vectorIndex = y + textureDimension * x;
		return textures[face.material.textureIndex].pixelArray[vectorIndex];

	}
}


void PictureData::traceRays()
{
//foreach pixel in image array:
	//call trace_ray() with appropriate parameters
	//Use value returend by trace_ray to update pixel colors.
	//cout << "Starting the loop through each pixel" << endl;
	for(int y = 0; y < height; y++)
	{
		//cout << "height:" << y << "/" << height << endl;
		for(int x = 0; x < width; x++)
		{
			PointType worldCoordinatesPoint = 
				Common::getPointFromVector(
				Common::getVectorFromPoint(viewingWindow.topLeft).addVectors(
				deltaH.multiplyVector(x)).addVectors(
				deltaV.multiplyVector(y)));
				
			//cout << "World Coordinates Point:" << worldCoordin

			RayType rayToTrace;

			if(isParallel)
			{
				//If it's parallel, we start the ray at the spot in our viewing window.
				rayToTrace.origin = worldCoordinatesPoint;
				rayToTrace.direction = eyeRay.direction.normalizeVector();

			}
			else
			{
				rayToTrace.origin = eyeRay.origin;
				rayToTrace.direction = Common::vectorFromHereToPoint(rayToTrace.origin,worldCoordinatesPoint).normalizeVector();
			}

			///assume ray starts in air.  So the index of refraction is 1.
			rayToTrace.insideObject = false;
			rayToTrace.indexOfRefraction = 1;

			pixelArray [ x + y*width ] = traceRay(rayToTrace, 1);
		}
	}
}



