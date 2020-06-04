#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

#define BUFFER_OFFSET(a) ((void*)(a))

// Vertex Array Object IDs, 0 is the first one and the last one is size.
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

// declare verteces (Used in both init and display)
const GLuint NumVertices = 6;

// Vertex Shader char array
static const char* vertex_shader_text =
"#version 450 core\n"
"layout ( location = 0 ) in vec4 vPosition;\n"
"void main ()\n"
"{\n"
"	gl_Position = vPosition;\n"
"}\n";

// Fragment Shader char array
static const char* fragment_shader_text =
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
	// vertices Info
	static const GLfloat vertice[NumVertices][2] =
	{
		{ -0.90,-0.90 }, // Triangle 1
		{  0.85, -0.90 },
		{ -0.90, 0.85 },
		{  0.90, 0.90 }, // Tirangle 2
		{  0.90, -0.85},
		{ -0.85, 0.90 }
	};

	// Gernerate vertex array object names.
	// VAOs is the place to store the name
	glCreateVertexArrays ( NumVAOs, VAOs );

	glCreateBuffers ( NumBuffers, Buffers );
	glNamedBufferStorage ( Buffers[ArrayBuffer], sizeof ( vertice ), vertice, 0 );

	GLuint vertex_shader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertex_shader, 1, &vertex_shader_text, NULL );
	glCompileShader ( vertex_shader );

	GLuint fragment_shader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragment_shader, 1, &fragment_shader_text, NULL );
	glCompileShader ( fragment_shader );
	
	GLuint program = glCreateProgram ();
	glAttachShader ( program, vertex_shader );
	glAttachShader ( program, fragment_shader );
	glLinkProgram ( program );
	glUseProgram ( program );
	

	glBindVertexArray ( VAOs[Triangles] );
	glBindBuffer ( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET ( 0 ) );
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
	glClearBufferfv ( GL_COLOR, 0, black );

	glBindVertexArray ( VAOs[Triangles] );

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