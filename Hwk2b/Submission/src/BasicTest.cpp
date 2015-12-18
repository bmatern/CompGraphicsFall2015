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

// Some Constant I'll use
float scaleX, scaleY
	, rotationX, rotationY
	, translationX, translationY
	, curMousePosX, curMousePosY
	, heldMousePosX, heldMousePosY
	;

GLuint program;

bool translateMode, rotateMode;

//forward declarations
void defaultValues();
void setUniformVariables();//int);




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

    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		scaleX = scaleX * .95;
		std::cout << "scaleX" << scaleX << std::endl;
		setUniformVariables();
	}
	else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		scaleX = scaleX * 1.05;
		std::cout << "scaleX" << scaleX << std::endl;
		setUniformVariables();
	}
	else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		scaleY = scaleY * 1.05;
		std::cout << "scaleY" << scaleY << std::endl;
		setUniformVariables();
	}
	else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		scaleY = scaleY * 0.95;
		std::cout << "scaleY" << scaleY << std::endl;
		setUniformVariables();
		//std::cout << heightMultiplier << std::endl;
	}

	else if(key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
	{
		//ALT should allow user to translate the model.
		if(action == GLFW_PRESS)
		{
			//record the mouse position.
			heldMousePosX = curMousePosX;
    		heldMousePosY = curMousePosY;
			translateMode = true;
			std::cout << "Alt pressed, translate mode is on." << std::endl;
		}
		else if(action == GLFW_RELEASE)
		{
			translateMode = false;
			std::cout << "Alt unpressed, translate mode is off." << std::endl;
		}
	}

	//This is hardcoded becasue i can't figure out how to get it right now.
	//TODO
	//GLint vertexShaderID = 1;
	//glGetIntegerv(GL_CURRENT_PROGRAM,&amp;id);
	//setUniformVariables();//ertexShaderID);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//holding the left mouse button should allow the user to rotate the object.

    //if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    //    glfwSetWindowShouldClose(window, GL_TRUE); 

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
    	if(action == GLFW_PRESS)
    	{
    		//record the mouse position
    		heldMousePosX = curMousePosX;
    		heldMousePosY = curMousePosY;
        	rotateMode = true;
        }
        else
        {
        	rotateMode = false;
        }
    } 
}

// for mouse position input
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Record the current mouse position.
	curMousePosX = xpos;
	curMousePosY = ypos;

	//Yeah, lets only rotate or translate.   One at a time please.
	if(rotateMode)
	{
		float xDif = curMousePosX - heldMousePosX;
		float yDif = curMousePosY - heldMousePosY;

		rotationX = xDif;
		rotationY = yDif;

		std::cout << "RotationMode" << rotationX << "," << rotationY << std::endl;

		setUniformVariables();
	}
	else if(translateMode)
	{
		float xDif = curMousePosX - heldMousePosX;
		float yDif = curMousePosY - heldMousePosY;

		translationX = xDif;
		translationY = yDif;

		std::cout << "TranslationMode" << translationX << "," << translationY << std::endl;

		setUniformVariables();
	}
	else
	{
		//I dunno
	}

}

// initialize some basic structures and geometry
typedef struct {
    float x, y;
} FloatType2D;

typedef struct {
    float x, y, z;
} FloatType3D;


const int nvertices = 4;

//----------------------------------------------------------------------------

void init( void )
{
    FloatType2D vertices[nvertices];

	FloatType3D colors[nvertices];
    GLuint vao[1], buffer, vertexLocation, colorLocation;//, program;
    
    //For my simple change, I'm gonna move these vertices around.
    /*vertices[0].x = -0.9;  vertices[0].y = -0.9;
    vertices[1].x =  0.9;  vertices[1].y = -0.6;
    vertices[2].x =  0.9;  vertices[2].y =  0.6;
    vertices[3].x = -0.9;  vertices[3].y =  0.9;*/

    //These are my changed vertices.
    
    /*vertices[0].x = -0.2;  vertices[0].y = -0.9;
    vertices[1].x =  0.4;  vertices[1].y = -0.2;
    vertices[2].x =  0.8;  vertices[2].y =  0.7;
    vertices[3].x = -0.9;  vertices[3].y =  0.6;*/
    
    vertices[0].x = -0.75;  vertices[0].y = -0.75;
    vertices[1].x =  0.75;  vertices[1].y = -0.75;
    vertices[2].x =  0.75;  vertices[2].y =  0.75;
    vertices[3].x = -0.75;  vertices[3].y =  0.75;

    //XYZ = RGB
    //These are red.
    colors[0].x = 1.0;  colors[0].y = 0.0;  colors[0].z = 0.0;
    colors[1].x = 0.0;  colors[1].y = 0.0;  colors[1].z = 1.0;
    colors[2].x = 0.0;  colors[2].y = 1.0;  colors[2].z = 0.0;
    colors[3].x = 1.0;  colors[3].y = 0.0;  colors[3].z = 1.0;
 

    
    // Create a vertex array object
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );

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
    glVertexAttribPointer( vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

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

	// Create the shaders
	init();

	//So this must be the main loop.  
	//It waits for events.
	while (!glfwWindowShouldClose(window)) {

        // draw graphics
		glClear( GL_COLOR_BUFFER_BIT );     // fills the window with the background color
		//glDrawArrays( GL_POINTS, 0, nvertices );    // draw a point at each vertex location
		//glDrawArrays( GL_LINES, 0, nvertices );    // draw a line between each vertex pair
		//glDrawArrays( GL_LINE_LOOP, 0, nvertices );    // draw a line between each successive vertex
		//glDrawArrays( GL_TRIANGLES, 0, nvertices );    // draw a triangle for each vertex triple
		glDrawArrays( GL_TRIANGLE_FAN, 0, nvertices );    // draw a triangle between the first vertex and each successive vertex pair
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
	//Defaults.  Push R for reset.
	translationX = 0;
	translationY = 0;
	scaleX = 1;
	scaleY = 1;
	rotationX = 0;
	rotationY = 0;

	translateMode = false;
	rotateMode = false;

	heldMousePosX = 0;
	heldMousePosY = 0;
	curMousePosX = 0;
	curMousePosY = 0;
}


void setUniformVariables()//GLint programID)
{
	//Here's where I"ll create a matrix and pass to the shader.  

	std::cout << "Inside SetUniformVariables. program ID is:" << program << std::endl;

	//I'm gonna make a matrix for each transform and multiply them together.
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	//This is the location of the matrix in the shader
	GLint MatrixID;
	MatrixID = glGetUniformLocation(program, "modelViewMatrix");
	std::cout << "MatrixID:" << MatrixID << std::endl;

	//I'll calculate theta for the rotation.  c++ cos() expects radians.
	//rotationX should be approximately on the scale of the right # of degrees, 
	// so convert pixel count to radians(Sure it makes sense)
	double theta = -rotationX * (M_PI/180);

	//fill in the interesting stuff on the scale matrix.
	scaleMatrix[0][0] = 1 * scaleX;
	scaleMatrix[1][1] = 1 * scaleY;

	//4 values for the rotation
	rotationMatrix[0][0] = cos(theta);
	rotationMatrix[1][0] = sin(theta);
	rotationMatrix[0][1] = -sin(theta);
	rotationMatrix[1][1] = cos(theta);

	//trsx = [0][2]  by itself this is the translated x term.
	// 1.0/400 is kind of an arbitrary multiplier.  Seems to work.
	translationMatrix[0][2] = translationX * (1.0/400);
	translationMatrix[1][2] = translationY * (-1.0/400);

	//First scale, then rotation, then translation.
	//This here's matrix multiplication.
	//For free.  Without paying.
	glm::mat4 transformedMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	for(int i = 0; i < 4; i++)
	{
			for(int j = 0; j < 4; j++)
			{
				std::cout << "Matrix:" << i << "," << j << ":" << transformedMatrix[i][j]<<std::endl;
			}
	}

	//Apply my new matrix to the transformation.
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &transformedMatrix[0][0]);
}

