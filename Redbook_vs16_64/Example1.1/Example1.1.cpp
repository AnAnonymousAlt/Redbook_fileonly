#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

#define BUFFER_OFFSET(a) ((void*)(a))

enum VAO_IDs
{
	Triangles, NumVAOs
};
enum Buffer_IDs
{
	ArrayBuffer, NumBuffers
};
enum Attrib_IDs
{
	vPosition = 0
};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;
GLuint vertex_shader, fragment_shader, program;

static const char* vertex_shader_text =
"#version 450 core\n"
"layout ( location = 0 ) in vec4 vPosition;\n"
"void main ()\n"
"{\n"
"	gl_Position = vPosition;\n"
"}\n";

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

	static const GLfloat vertice[NumVertices][2] =
	{
		{ -0.90,-0.90 }, // Triangle 1
		{  0.85, -0.90 },
		{ -0.90, 0.85 },
		{  0.90, 0.90 }, // Tirangle 2
		{  0.90, -0.85},
		{ -0.85, 0.90 }
	};

	glCreateVertexArrays ( NumVAOs, VAOs );

	glCreateBuffers ( NumBuffers, Buffers );
	glNamedBufferStorage ( Buffers[ArrayBuffer], sizeof ( vertice ), vertice, 0 );

	vertex_shader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertex_shader, 1, &vertex_shader_text, NULL );
	glCompileShader ( vertex_shader );

	fragment_shader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragment_shader, 1, &fragment_shader_text, NULL );
	glCompileShader ( fragment_shader );
	
	program = glCreateProgram ();
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