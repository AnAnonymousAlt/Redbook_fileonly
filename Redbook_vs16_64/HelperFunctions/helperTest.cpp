#ifndef _GL_DEF
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define _GL_DEF
#endif // !_GL_DEF

#include <fstream>
#include <string>

using namespace std;

enum VAO_ID
{
	Triangles, numVAOs
};
enum BUFFER_ID
{
	ArrayBuffer, numBuffers
};
enum ATTRIB_ID
{
	vPosition = 0
};

GLuint VAOs[numVAOs];
GLuint buffers[numBuffers];
const GLuint numVertices = 3;

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

	GLuint program = glCreateProgram ();
	std::ifstream inv ( "vertext.glsl" );
	std::string vertShaderString = std::string ( std::istreambuf_iterator<char> ( inv ),
												 std::istreambuf_iterator<char> () );
	const char* vertShaderText = vertShaderString.c_str ();

	std::ifstream inf ( "fragment.glsl" );
	std::string fragShaderString = std::string ( std::istreambuf_iterator<char> ( inf ),
												 std::istreambuf_iterator<char> () );
	const char* fragShaderText = fragShaderString.c_str ();


	GLuint vertShader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertShader, 1, &vertShaderText, NULL );
	glCompileShader ( vertShader );

	GLuint fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, &fragShaderText, NULL );
	glCompileShader ( fragShader );


	glAttachShader ( program, vertShader );
	glAttachShader ( program, fragShader );
	glLinkProgram ( program );
	glUseProgram ( program );

	glBindVertexArray ( VAOs[Triangles] );
	glBindBuffer ( GL_ARRAY_BUFFER, buffers[ArrayBuffer] );
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );
	glEnableVertexAttribArray ( vPosition );

}
void display ()
{
	static const float black[] = { 0.0f, 0.0f, 1.0f, 1.0f };
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