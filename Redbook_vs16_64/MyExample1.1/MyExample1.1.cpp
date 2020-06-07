// My Example1.1 from Redbook

#ifndef _GL_DEF
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define _GL_DEF
#endif // !_GL_DEF

#include "shader_loader.h"

using namespace std;

// constexpr, substitue #DEFINE
// a void pointer for buffer offset
constexpr void *bufferOffset ( int a )
{
	return ( void * ) a;
}


// Vertex Array Object IDs, 0 is the first one and the last one is size.
/*
What is VAO?
Basicly, it is a place to store state info.

"Vertex Array Objects store no data. They are a class of objects known as containers, like Framebuffer Objects. You may attach/associate other objects with them, but they never store data themselves. As such they are not a context shareable resource.

Basically Vertex Array Objects encapsulate vertex array state in OpenGL 3.0. Beginning with OpenGL 3.1 (in lieu of GL_ARB_compatibility) and OpenGL 3.2+ Core profiles, you must have a non-zero VAO bound at all times for commands like glVertexAttribPointer (...) or glDrawArrays (...) to function. The bound VAO forms the necessary context for these commands, and stores the state persistently."
https://stackoverflow.com/a/21652930
*/
enum VAO_ID
{
	Triangles, numVAOs
};

// Buffer IDs
enum BUFFER_ID
{
	ArrayBuffer, numBuffers
};

// Attribute IDs
/*
What is Generic Vertex Attribute?
It's vertex information and the input values to vertex shaders.
*/
enum ATTRIB_ID
{
	vPosition = 0
};
// Vertex Array Object Array
// Here we only have one VAO, triangles.
GLuint VAOs[numVAOs];

// Buffer Array
// only one buffer, ArrayBuffer
GLuint buffers[numBuffers];

// Declare verteces (Used in both init and display)
const GLuint numVertices = 3;

/*
static const char* vertShaderText =
"#version 450 core\n"
"layout ( location = 0 ) in vec4 vPosition;\n"
"void main()\n"
"{\n"
"gl_Position = vPosition;\n"
"}\n";

static const char* fragShaderText =
"#version 450 core\n"
"layout ( location = 0 ) out vec4 fColor;\n"
"void main()\n"
"{	fColor = vec4 ( 0.5, 0.1, 0.6, 1.0 );	}\n";
*/


/// <summary>
/// My init function
/// </summary>
void init ()
{
	// Vertices Info
	static const GLfloat vertices[numVertices][2] =
	{
		{ -0.5,  0.5 }, // Triangle 1
		{ -0.5, -0.5 },
		{  0.5, -0.5 }
	};
	// Create vertex array object
	// NumVAOs is the number of vertex array object to create. Here is 1
	// VAOs is the place to store the names of new vertex array object 
	// Name is an Opengl thing like a pointer
	// After this, one name of a new array object is store in VAOs
	glCreateVertexArrays ( numVAOs, VAOs );
	// Create buffer objects
	// NumBuffers is the number of buffer object to create. Here is 1
	// Buffers is array which names of the new buffer objects are stored
	// After this, 1 name of a new buffer object is stored in Buffers
	glCreateBuffers ( numBuffers, buffers );
	// Creates and initializes a buffer's immutable data store
	// Here we store our vertices into the buffer
	// first argument is the name of the buffer object
	// Here is the one we just created and stored in Buffers
	// last argument is the flag
	glNamedBufferStorage ( buffers[ArrayBuffer], sizeof ( vertices ), 
						   &vertices, 0 );

	/*
	// shader loader
	ifstream inv ( "vertex.glsl" );
	string vertShaderString = string ( istreambuf_iterator<char> ( inv ), istreambuf_iterator<char> () );
	const char* vertShaderText = vertShaderString.c_str ();

	ifstream inf ( "fragment.glsl" );
	string fragShaderString = string ( istreambuf_iterator<char> ( inf ), istreambuf_iterator<char> () );
	const char* fragShaderText = fragShaderString.c_str ();


	GLuint vertShader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertShader, 1, &vertShaderText, NULL);
	glCompileShader ( vertShader );

	GLuint fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, &fragShaderText, NULL );
	glCompileShader ( fragShader );


	GLuint program = glCreateProgram ();
	glAttachShader ( program, vertShader );
	glAttachShader ( program, fragShader );
	glLinkProgram ( program );
	glUseProgram ( program );
	*/

	// Create a program object
	// A program object is an object to which shader objects can be attached.
	// This provides a mechanism to specify the shader objects that will be linked to create a program
	GLuint program = glCreateProgram ();
	// Inline function in shader_loader.h
	loadShader ( program );

	// Binds the vertex array object. Make this Vertex array current
	glBindVertexArray ( VAOs[Triangles] );
	// Bind the buffer to the specified target
	// Here makes the Buffers[ArrayBuffer] the current vertex attribute
	// Input to vertex shader
	glBindBuffer ( GL_ARRAY_BUFFER, buffers[ArrayBuffer] );
	// Define a generic vertex attribute data by index in the array
	// Tell the server how to specify how the bound buffer should be decoded
	// 2 is the number of values for each vertex in our array
	// FALSE is because that we don't want to constrain it to [-1, 1], either we use float instead of int here
	// 0 is for 0 interval between values
	// last argument is 0 because the value starts at the first location
	// There is an active array as the activate VAO and buffer
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 
							0, bufferOffset ( 0 ) );
	// Enable a generic vertex attribute specified by index
	glEnableVertexAttribArray ( vPosition );

}


/// <summary>
/// main display function
/// </summary>
void display ()
{
	static const float background[] = { 0.2f, 0.0f, 0.1f, 1.0f };
	// Clear individual buffers of a frame buffer
	glClearBufferfv ( GL_COLOR, 0, background );

	glBindVertexArray ( VAOs[Triangles] );

	// Render primitives from array data
	glDrawArrays ( GL_TRIANGLES, 0, numVertices );
}


/// <summary>
/// Main
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main ( int argc, char **argv )
{
	// Init glfw
	glfwInit ();

	// create a window
	GLFWwindow *window = glfwCreateWindow ( 640, 480, "Triangle", NULL, NULL );

	// make the window current
	glfwMakeContextCurrent ( window );

	// Init gl3w
	gl3wInit ();

	// My init
	init ();

	// main loop
	while ( !glfwWindowShouldClose ( window ) )
	{
		// main display
		display ();

		// Swap buffers
		glfwSwapBuffers ( window );

		// Events
		glfwPollEvents ();
	}

	// Destroy window
	glfwDestroyWindow ( window );

	// Terminate
	glfwTerminate ();
	return 0;
}