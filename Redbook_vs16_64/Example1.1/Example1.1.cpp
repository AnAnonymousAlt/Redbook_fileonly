// This is the x64 version with inline shader code of Example 1.1 in Redbook

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

#define BUFFER_OFFSET(a) ((void*)(a))

// Vertex Array Object IDs, 0 is the first one and the last one is size.
/*
What is VAO? 
Basicly, it is a place to store state info.

"Vertex Array Objects store no data. They are a class of objects known as containers, like Framebuffer Objects. You may attach/associate other objects with them, but they never store data themselves. As such they are not a context shareable resource.

Basically Vertex Array Objects encapsulate vertex array state in OpenGL 3.0. Beginning with OpenGL 3.1 (in lieu of GL_ARB_compatibility) and OpenGL 3.2+ Core profiles, you must have a non-zero VAO bound at all times for commands like glVertexAttribPointer (...) or glDrawArrays (...) to function. The bound VAO forms the necessary context for these commands, and stores the state persistently."
https://stackoverflow.com/a/21652930
*/
enum VAO_IDs
{
	Triangles, NumVAOs
};

// Buffer IDs
enum Buffer_IDs
{
	ArrayBuffer, NumBuffers
};

// Attribute IDs
/*
What is Generic Vertex Attribute?
It's vertex information and the input values to vertex shaders.
*/
enum Attrib_IDs
{
	vPosition = 0
};


// Vertex Array Object Array
// Here we only have one VAO, triangles.
GLuint VAOs[NumVAOs];
// Buffer Array
// only one buffer, ArrayBuffer
GLuint Buffers[NumBuffers];

// Declare verteces (Used in both init and display)
const GLuint NumVertices = 6;

// Vertex Shader char array
// NOTE: vPostion is a generic vertex attribute and it will be defined later
// layout () is a layout qualifier and provides meta-data for our variable declaration.
// Here we set the location of vPosition to 0.
// So as the Attrib_ID of vPosition
const char* vertex_shader_text =
"#version 450 core\n"
"layout ( location = 0 ) in vec4 vPosition;\n"
"void main ()\n"
"{\n"
"	gl_Position = vPosition;\n"
"}\n";

// Fragment Shader char array
const char* fragment_shader_text =
"#version 450 core\n"
"layout (location = 0) out vec4 fColor;\n"
"void main()\n"
"{\n"
"	fColor = vec4(0.5, 0.4, 0.8, 1.0);\n"
"}\n";

//----------------------------
//
// Init
//
void
init ( void )
{
	// Vertices Info
	static const GLfloat vertice[NumVertices][2] =
	{
		{ -0.90,-0.90 }, // Triangle 1
		{  0.85, -0.90 },
		{ -0.90, 0.85 },
		{  0.90, 0.90 }, // Tirangle 2
		{  0.90, -0.85},
		{ -0.85, 0.90 }
	};

	// Create vertex array object
	// NumVAOs is the number of vertex array object to create. Here is 1
	// VAOs is the place to store the names of new vertex array object 
	// Name is an Opengl thing like a pointer
	// After this, one name of a new array object is store in VAOs
	glCreateVertexArrays ( NumVAOs, VAOs );

	// Create buffer objects
	// NumBuffers is the number of buffer object to create. Here is 1
	// Buffers is array which names of the new buffer objects are stored
	// After this, 1 name of a new buffer object is stored in Buffers
	glCreateBuffers ( NumBuffers, Buffers );
	// Creates and initializes a buffer's immutable data store
	// Here we store our vertices into the buffer
	// first argument is the name of the buffer object
	// Here is the one we just created and stored in Buffers
	// last argument is the flag
	glNamedBufferStorage ( Buffers[ArrayBuffer], sizeof ( vertice ), vertice, 0 );

	// Create a shader object
	GLuint vertex_shader = glCreateShader ( GL_VERTEX_SHADER );
	// Replace the source code in a shader object
	// 1 is the number of string array at argument 3
	// NULL is the array of string length
	glShaderSource ( vertex_shader, 1, &vertex_shader_text, NULL );
	// Compile a shader object
	glCompileShader ( vertex_shader );

	// Same thing as the vertex shader
	GLuint fragment_shader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragment_shader, 1, &fragment_shader_text, NULL );
	glCompileShader ( fragment_shader );
	
	// Create a program object
	// A program object is an object to which shader objects can be attached.
	// This provides a mechanism to specify the shader objects that will be linked to create a program
	GLuint program = glCreateProgram ();
	// Attaches a shader object to a program object
	// Shaders that are to be linked together in a program object must first be attached to that program object.
	// This action of attaching indicates that shader will be included in link operations that will be performed on program.
	glAttachShader ( program, vertex_shader );
	glAttachShader ( program, fragment_shader );
	// Links a program object
	// As a result of a successful link operation, all active user-defined uniform variables belonging to program will be initialized to 0.
	// And each of the program object's active uniform variables will e assigned a location that can be queried by calling glGetUniformLocation.
	glLinkProgram ( program );
	// Install a program object as part of current rendering state
	// Any shader objects of type GL_VERTEX_SHADER, GL_GEOMETRY_SHADER and GL_FRAGMENT_SHADER attached to program are used to create excutable that run on the corresbonding programmable processor.
	glUseProgram ( program );
	
	// Binds the vertex array object. Make this Vertex array current
	glBindVertexArray ( VAOs[Triangles] );
	// Bind the buffer to the specified target
	// Here makes the Buffers[ArrayBuffer] the current vertex attribute
	// Input to vertex shader
	glBindBuffer ( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
	// Define a generic vertex attribute data by index in the array
	// Tell the server how to specify how the bound buffer should be decoded
	// 2 is the number of values for each vertex in our array
	// FALSE is because that we don't want to constrain it to [-1, 1], either we use float instead of int here
	// 0 is for 0 interval between values
	// last argument is 0 because the value starts at the first location
	// There is an active array as the activate VAO and buffer
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );
	// Enable a generic vertex attribute specified by index
	glEnableVertexAttribArray ( vPosition );
}

//------------------------------
//
// Display
//
void
display ()
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// Clear individual buffers of a frame buffer
	glClearBufferfv ( GL_COLOR, 0, black );

	// Render primitives from array data
	glDrawArrays ( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------
//
// Main
//
int
main ( int argc, char** argv )
{
	glfwInit ();

	GLFWwindow* window = glfwCreateWindow ( 640, 480, "Triangles", NULL, NULL );

	glfwMakeContextCurrent ( window );
	gl3wInit ();

	init ();

	while ( !glfwWindowShouldClose ( window ) )
	{
		display ();
		glfwSwapBuffers ( window );
		glfwPollEvents ();
	}

	glfwDestroyWindow ( window );

	glfwTerminate ();
	return 0;
}