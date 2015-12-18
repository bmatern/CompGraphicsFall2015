// identity vertex shader
#version 150
in vec4 vertex_position;
in vec4 vertex_color;
out vec4 vcolor;

//uniform float scaleX;
//uniform float scaleY;
uniform mat4 modelViewMatrix;

//uniform float rotationX;
//uniform float rotationY;

void main()  
{
	// In the vertex shader, we want to create a matrix, do the transform.  

	//Any 2D transformation that results from an
	//arbitrary combination of translation, rotation, and
	//scaling operations can be expressed as a single matrix.

	//x' = x ⋅rsxx + y ⋅ rsxy + trsx
	//y' = x ⋅rsyx + y ⋅ rsyy + trsy

	// where the four elements rsjk are multiplicative
	// terms that depend on the rotations and scalings
	// only, and the two elements trsj are additive terms
	// that can depend on all of the transformations

	// That sounds easy.

	float x = vertex_position.x;
	float y = vertex_position.y;

	////rsxx is index [0][0], it represents the scale of the x term 
	//myIdentityMatrix[0][0] = 1 * scaleX;
	//rsyx = [1][0] I think
	//rsxy = [0][1]
	//rsyy is index [1][1], scale y
	//myIdentityMatrix[1][1] = 1 * scaleY;
	//trsx = [0][2]
	//trsy = [1][2]

	float rsxx = 1;
	rsxx = modelViewMatrix[0][0];
	float rsyy = 1;
	rsyy = modelViewMatrix[1][1];
	float rsyx = 0;
	rsyx = modelViewMatrix[1][0];
	float rsxy = 0;
	rsxy = modelViewMatrix[0][1];
	float trsx = 0;
	trsx = modelViewMatrix[0][2];
	float trsy = 0;
	trsy = modelViewMatrix[1][2];

	float xP = x * rsxx + y * rsxy + trsx;
	float yP = x * rsyx + y * rsyy + trsy; 

	gl_Position = vertex_position;  // pass on the vertex position unchanged
	gl_Position.x = xP;
	gl_Position.y = yP;


	//This is how a website proposed that I do the transform.
	//I wonder what he's doing with these homogenous coordinates.  
	//This will do a matrix multiplication
	//vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector
    //gl_Position = MVP * v;

    //vec4 v = vec4(vertex_position,1); // Transform an homogeneous 4D vector
    //gl_Position = modelViewMatrix * v;
    //gl_Position = modelViewMatrix * vertex_position;




	/*if(x < 0)
	{
		gl_Position.x = 0;
	}*/



	vcolor = vertex_color;
	
}
