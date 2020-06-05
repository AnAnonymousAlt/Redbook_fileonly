#ifndef _GL_DEF
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define _GL_DEF
#endif // !_GL_DEF

#include <GL/helper_functions.h>

using namespace std;

enum VAO_ID { Triangles, numVAOs };
enum BUFFER_ID { ArrayBuffer, numBuffers };
enum ATTRIB_ID { vPosition = 0 };

GLuint VAOs[numVAOs];
GLuint buffers[numBuffers];
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


void init ()
{
	static const GLfloat vertices[numVertices][2] =
	{
		{ -0.5,  0.5 },
		{ -0.5, -0.5 },
		{  0.5, -0.5 }
	};

	glCreateVertexArrays ( numVAOs, VAOs );
	glCreateBuffers ( numBuffers, buffers );
	glNamedBufferStorage ( buffers[ArrayBuffer], sizeof ( vertices ), &vertices, 0 );

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

	GLuint program = glCreateProgram ();
	loadShader ( program );
	
	glBindVertexArray ( VAOs[Triangles] );
	glBindBuffer ( GL_ARRAY_BUFFER, buffers[ArrayBuffer] );
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );
	glEnableVertexAttribArray ( vPosition );

}
void display ()
{
	static const float black[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv ( GL_COLOR, 0, black );

	glBindVertexArray ( VAOs[Triangles] );

	glDrawArrays ( GL_TRIANGLES, 0, numVertices );
}
int main ( int argc, char** argv )
{
	glfwInit ();

	GLFWwindow* window = glfwCreateWindow ( 640, 480, "Triangle", NULL, NULL );

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