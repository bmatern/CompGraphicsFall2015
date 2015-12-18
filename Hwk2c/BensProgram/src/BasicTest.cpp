// Basic OpenGL program
// Based on example code from: Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL (6th Edition), by Ed Angel

#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>

//Lets try using GLM to make some stuff.
#include "glm/glm.hpp"

//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <math.h> 

// This file contains the code that reads the shaders from their files and compiles them
#include "ShaderStuff.hpp"

const int nvertices = 8;

glm::mat4 viewTransMatrix;
glm::mat4 projNormMatrix;
glm::vec4 upVector, cameraLocation, viewDirection;
double near, far, top, bottom, left, right, aspectRatio;

GLuint program;

//forward declarations
void defaultValues();
void setUniformVariables();//int);
void step(bool);
void turn(bool);
double dotProduct(glm::vec4, glm::vec4);
glm::vec4 crossProduct(glm::vec4, glm::vec4);
double vectorLength(glm::vec4);
glm::vec4 normalizeVector(glm::vec4);
glm::vec4 multiplyVector(glm::vec4, double);

// function that is called whenever an error occurs
static void
error_callback(int error, const char* description){
    fputs(description, stderr);  // write the error description to stderr
}

// function that is called whenever a keyboard event occurs; defines how keyboard input will be handled
static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // checks to see if the escape key was pressed
    {
        glfwSetWindowShouldClose(window, GL_TRUE);  // closes the window
    }

    //R is for reset
    else if(key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		std::cout << "Reset to Defaults" << std::endl;
		defaultValues();
		setUniformVariables();
	}

	else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		step(true);
		std::cout << "Step Forward" << std::endl;
		setUniformVariables();
	}
	else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		step(false);
		std::cout << "Step Backward" << std::endl;
		setUniformVariables();
	}
	else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		turn(false);
		std::cout << "Turn left" << std::endl;
		setUniformVariables();	
	}
	else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		turn(true);
		std::cout << "Turn Right" << std::endl;
		setUniformVariables();	
	}

}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

// for mouse position input
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

static void window_resize_callback(GLFWwindow* window, int width, int height)
{
	std::cout <<"Window Resize Callback.  Width:" << width << " , Height:" << height << std::endl;
	//GLint dim = std::min(width,height);
	//std::cout <<"dim"<<dim<<std::endl;
	aspectRatio = (1.0 * width) / height;
	std::cout <<"AspRat"<<aspectRatio<<std::endl;

	if(height > width)
	{
		//If resizing causes the aspect_ratio to become less than 1, we will now have height > width, 
		//and this means that the window should now show more content vertically.
		left = -1;
		right = 1;
		top = 1 / aspectRatio;
		bottom = -1 / aspectRatio;
	}
	else
	{
		left = -1 * aspectRatio;
		right = 1 * aspectRatio;
		top = 1;
		bottom = -1;


	}


	glViewport(0,0, 2*width, 2*height);
	setUniformVariables();	
}

//----------------------------------------------------------------------------

void init( void )
{
    glm::vec4 vertices[nvertices];

	glm::vec3 colors[nvertices];
    GLuint vao[1], buffer, vertexLocation, colorLocation;//, program;
    
    //Here's my scene.  2 brightly "quadrilaterals" or whatever.  

    vertices[0].x = -0.75;  vertices[0].y =  0.75;  vertices[0].z = -0.50;  vertices[0].w = 1;
    vertices[1].x = -0.25;  vertices[1].y =  0.75;  vertices[1].z = -0.50;  vertices[1].w = 1;
    vertices[2].x = -0.75;  vertices[2].y = -0.75;  vertices[2].z = -0.50;  vertices[3].w = 1;
    vertices[3].x = -0.25;  vertices[3].y = -0.75;  vertices[3].z = -0.50;  vertices[2].w = 1;

    //XYZ = RGB
    colors[0].x = 1.0;  colors[0].y = 1.0;  colors[0].z = 0.0;
    colors[1].x = 1.0;  colors[1].y = 0.0;  colors[1].z = 1.0;
    colors[2].x = 1.0;  colors[2].y = 1.0;  colors[2].z = 0.0;
    colors[3].x = 1.0;  colors[3].y = 0.0;  colors[3].z = 1.0;


    vertices[4].x =  0.25;  vertices[4].y =  0.75;  vertices[4].z =  0.50;  vertices[4].w = 1;
    vertices[5].x =  0.75;  vertices[5].y =  0.75;  vertices[5].z =  0.50;  vertices[5].w = 1;
    vertices[6].x =  0.25;  vertices[6].y = -0.75;  vertices[6].z =  0.50;  vertices[6].w = 1;
    vertices[7].x =  0.75;  vertices[7].y = -0.75;  vertices[7].z =  0.50;  vertices[7].w = 1;

    colors[4].x = 0.0;  colors[4].y = 0.5;  colors[4].z = 1.0;
    colors[5].x = 0.0;  colors[5].y = 0.3;  colors[5].z = 1.0;
    colors[6].x = 0.0;  colors[6].y = 1.0;  colors[6].z = 1.0;
    colors[7].x = 0.0;  colors[7].y = 0.2;  colors[7].z = 1.0;
    
    // Create a vertex array object
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );

    // Enable depth test for 3d objects.
    glEnable(GL_DEPTH_TEST);

    // Create and initialize a buffer object
    //glGenBuffers( 1, &buffer );
    //glBindBuffer( GL_ARRAY_BUFFER, buffer );
    //glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    // New code for buffers with colors
    glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices) +sizeof(colors), vertices, GL_STATIC_DRAW );
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    
    // Define the names of the shader files
    std::stringstream vshader, fshader;
    vshader << SRC_DIR << "/vshader1.glsl";
    fshader << SRC_DIR << "/fshader1.glsl";
    
    // Load the shaders and use the resulting shader program
    program = InitShader( vshader.str().c_str(), fshader.str().c_str() );
    
    // Initialize the vertex position attribute from the vertex shader
    vertexLocation = glGetAttribLocation( program, "vertex_position" );
    glEnableVertexAttribArray( vertexLocation );
    glVertexAttribPointer( vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    //New attribute locations for colors
    colorLocation = glGetAttribLocation( program, "vertex_color" );
	glEnableVertexAttribArray( colorLocation );
	glVertexAttribPointer( colorLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)) );
    
    // Define OpenGL state variables
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
    //glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);
    //glEnable (GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPointSize(10.0);  // huge points
    //glLineWidth(5.0);  // fat lines

    //I'll try attaching the uniform variables here.  Maybe I only need to do it once.
	//GLint vertexShaderID = program;
	setUniformVariables();//program);
}

//----------------------------------------------------------------------------

int main(void)
{
	defaultValues();

	GLFWwindow* window;
    
    // Define the error callback function (one of the few things that can precede glfwInit)
    glfwSetErrorCallback(error_callback);
    
    // Initialize GLFW (performs platform-specific initialization)
    if (!glfwInit()) exit(EXIT_FAILURE);
    
    // Ask for OpenGL 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Use GLFW to open a window within which to display your graphics
	window = glfwCreateWindow(500, 500 , "BensProgram", NULL, NULL);
	
    // Verify that the window was successfully created; if not, print error message and terminate
    if (!window)
	{
        printf("GLFW failed to create window; terminating\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
	}
    
	glfwMakeContextCurrent(window); // makes the newly-created context current
	glfwSwapInterval(1);  // tells the system to wait to swap buffers until monitor refresh has completed; necessary to avoid tearing

    // Define the keyboard callback function
    glfwSetKeyCallback(window, key_callback);

    // Define the mouse button callback function.
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Define the curser/mouse position callback.
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Window Size Callback
    glfwSetWindowSizeCallback(window, window_resize_callback);

	// Create the shaders
	init();

	//So this must be the main loop.  
	//It waits for events.
	while (!glfwWindowShouldClose(window)) {

        // draw graphics
		//glClear( GL_COLOR_BUFFER_BIT );     // fills the window with the background color
		//glClear( GL_DEPTH_BUFFER_BIT);  //Clear the Z buffer
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Do both at the same time, i think.
		//glDrawArrays( GL_POINTS, 0, nvertices );    // draw a point at each vertex location
		//glDrawArrays( GL_LINES, 0, nvertices );    // draw a line between each vertex pair
		//glDrawArrays( GL_LINE_LOOP, 0, nvertices );    // draw a line between each successive vertex
		//glDrawArrays( GL_TRIANGLES, 0, nvertices );    // draw a triangle for each vertex triple
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );    // draw a triangle between the first vertex and each successive vertex pair
		glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );    // draw a triangle between the first vertex and each successive vertex pair
		
		//glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );    // draw a triangle between the first vertex and each successive vertex pair
		//glDrawArrays( GL_TRIANGLE_FAN, 4, 4 );    // draw a triangle between the first vertex and each successive vertex pair
		

		glFlush();	

        glfwSwapBuffers(window);  // swap buffers
        glfwPollEvents();  // check for events

	} // end graphics loop

	// Clean up
	glfwDestroyWindow(window);
	glfwTerminate();  // destroys any remaining objects, frees resources allocated by GLFW
	exit(EXIT_SUCCESS);

} // end main

void defaultValues()
{
	glm::mat4 viewTransMatrix = glm::mat4(1.0f);

	upVector.x = 0;
	upVector.y = 1;
	upVector.z = 0;
	upVector.w = 0;
	upVector = normalizeVector(upVector);

	cameraLocation.x = 0;
	cameraLocation.y = 0;
	cameraLocation.z = 1;
	cameraLocation.w = 1;

	viewDirection.x = 0;
	viewDirection.y = 0;
	viewDirection.z = -1;
	viewDirection.w = 0;
	viewDirection = normalizeVector(viewDirection);

	near = .3;
	far = 2;
	top = 1;
	bottom = -1;
	left = -1;
	right = 1;

}


void setUniformVariables()//GLint programID)
{
	//Here's where I"ll create a matrix and pass to the shader.  

	std::cout << "Inside SetUniformVariables. program ID is:" << program << std::endl;

	//This is the location of the matrix in the shader
	GLint viewTransMatrixID, projNormMatrixID;
	//MatrixID = glGetUniformLocation(program, "modelViewMatrix");
	viewTransMatrixID = glGetUniformLocation(program, "viewTransMatrix");
	std::cout << "viewTransMatrixID:" << viewTransMatrixID << std::endl;

	glm::vec4 n = normalizeVector(multiplyVector(viewDirection,-1));
	glm::vec4 u = normalizeVector(crossProduct(upVector,n));
	glm::vec4 v = normalizeVector(crossProduct(n,u));//90% sure this one should be normalized.
	double dx = -1 * dotProduct(cameraLocation,u);
	double dy = -1 * dotProduct(cameraLocation,v);
	double dz = -1 * dotProduct(cameraLocation,n);

	glm::mat4 viewTransMatrix = glm::mat4(1.0f);

	/*
	 ux uy uz dx
V =  vx vy vz dy
     nx ny nz dz
      0  0  0  1

where n = –normalized_view_direction,
u = up × n (normalized), 
v = n × u, and
dx = –(eye ⋅ u),
dy = –(eye ⋅ v), 
dz = –(eye ⋅ n).

*/

	/*viewTransMatrix[0][0] = u.x;
	viewTransMatrix[1][0] = v.x;
	viewTransMatrix[2][0] = n.x;

	viewTransMatrix[0][1] = u.y;
	viewTransMatrix[1][1] = v.y;
	viewTransMatrix[2][1] = n.y;

	viewTransMatrix[0][2] = u.z;
	viewTransMatrix[1][2] = v.z;
	viewTransMatrix[2][2] = n.z;

	viewTransMatrix[0][3] = dx;
	viewTransMatrix[1][3] = dy;
	viewTransMatrix[2][3] = dz;*/

    //This is column major mode.  The first index is column.
	viewTransMatrix[0][0] = u.x;
	viewTransMatrix[0][1] = v.x;
	viewTransMatrix[0][2] = n.x;

	viewTransMatrix[1][0] = u.y;
	viewTransMatrix[1][1] = v.y;
	viewTransMatrix[1][2] = n.y;

	viewTransMatrix[2][0] = u.z;
	viewTransMatrix[2][1] = v.z;
	viewTransMatrix[2][2] = n.z;

	viewTransMatrix[3][0] = dx;
	viewTransMatrix[3][1] = dy;
	viewTransMatrix[3][2] = dz;

	projNormMatrixID = glGetUniformLocation(program, "projNormMatrix");
	std::cout << "projNormMatrixID:" << projNormMatrixID << std::endl;

	projNormMatrix = glm::mat4(1.0f);

	projNormMatrix[0][0] = (2*near)/(right-left);
	projNormMatrix[1][1] = (2*near)/(top-bottom);
	projNormMatrix[2][0] = (right+left)/(right-left);
	projNormMatrix[2][1] = (top+bottom)/(top-bottom);
	projNormMatrix[2][2] = -(far + near)/(far-near);
	projNormMatrix[2][3] = -1;
	projNormMatrix[3][2] = (-2 * far * near)/(far- near);
	projNormMatrix[3][3] = 0;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << "ViewTransMatrix:" << i << "," << j << ":" << viewTransMatrix[i][j]<<std::endl;
		}
	}
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << "projNormMatrix:" << i << "," << j << ":" << projNormMatrix[i][j]<<std::endl;
		}
	}
	std::cout << "Eye location: " << cameraLocation.x << "," << cameraLocation.y << "," << cameraLocation.z << std::endl;
	std::cout << "View Direction: " << viewDirection.x << "," << viewDirection.y << "," << viewDirection.z << std::endl;

	//Apply my new matrix to the transformation.
	glUniformMatrix4fv(viewTransMatrixID, 1, GL_FALSE, &viewTransMatrix[0][0]);
	glUniformMatrix4fv(projNormMatrixID, 1, GL_FALSE, &projNormMatrix[0][0]);
}

void step(bool forward)
{
	double shiftValue = 
	forward ?  .05
	        : -.05 
	;

	//std::cout << "Old location: " << cameraLocation.x << "," << cameraLocation.y << "," << cameraLocation.z << std::endl;
	cameraLocation.x = cameraLocation.x + shiftValue * viewDirection.x;
	cameraLocation.y = cameraLocation.y + shiftValue * viewDirection.y;
	cameraLocation.z = cameraLocation.z + shiftValue * viewDirection.z;
	//std::cout << "New location: " << cameraLocation.x << "," << cameraLocation.y << "," << cameraLocation.z << std::endl;
	
}

void turn(bool right)
{
	double degreeTurn = 
	right ? -2.0
	      :  2.0 
	;

	//Convert from degrees to rads
	double radTurn = degreeTurn * M_PI / 180;

	glm::mat4 turnMatrix = glm::mat4(1.0f);

	turnMatrix[0][0] = cos(radTurn);
	turnMatrix[0][2] = -1 * sin(radTurn);
	turnMatrix[2][0] = sin(radTurn);
	turnMatrix[2][2] = cos(radTurn);

	viewDirection = turnMatrix * viewDirection;

	//std::cout << "Old location: " << cameraLocation.x << "," << cameraLocation.y << "," << cameraLocation.z << std::endl;
	//cameraLocation.x = cameraLocation.x + shiftValue * viewDirection.x;
	//cameraLocation.y = cameraLocation.y + shiftValue * viewDirection.y;
	//cameraLocation.z = cameraLocation.z + shiftValue * viewDirection.z;
	//std::cout << "New location: " << cameraLocation.x << "," << cameraLocation.y << "," << cameraLocation.z << std::endl;
	
}

double dotProduct(glm::vec4 firstVector, glm::vec4 secondVector)
{
	//Calculate dot product of 2 vectors
	//s = x1 x2 + y1 y2 + z1 z2
	double results = 
		firstVector.x * secondVector.x +
		firstVector.y * secondVector.y +
		firstVector.z * secondVector.z;
	return results;
}

glm::vec4 crossProduct(glm::vec4 firstVector, glm::vec4 secondVector)
{
	//calculate cross product firstVector X secondVector
	//u x v = 
		// (uy * vz - uz * vy 
		//, uz * vx - ux * vz 
		//, ux * vy - uy * vx)
	glm::vec4 results;
	results.x = (firstVector.y * secondVector.z) - (firstVector.z * secondVector.y);
	results.y = (firstVector.z * secondVector.x) - (firstVector.x * secondVector.z);
	results.z = (firstVector.x * secondVector.y) - (firstVector.y * secondVector.x);
	results.w = 0;
	return results;
}

double vectorLength(glm::vec4 vector)
{
	//calculate the length of a vector
	//sqrt(x^2 + y^2 + z^2)
	return sqrt(vector.x * vector.x
		+ vector.y * vector.y
		+ vector.z * vector.z);
}

glm::vec4 normalizeVector(glm::vec4 vector)
{
	//normalize the vector to length 1.
	double normalizeScalar = 1.0 / vectorLength(vector);
	glm::vec4 results = multiplyVector(vector, normalizeScalar);
	return results;
}

glm::vec4 multiplyVector(glm::vec4 vector, double scalar)
{
	//Multiply a vector by a scalar
	glm::vec4 results;	
	results.x = vector.x * scalar;
	results.y = vector.y * scalar;
	results.z = vector.z * scalar;
	results.w = 0;	
	return results;
}

