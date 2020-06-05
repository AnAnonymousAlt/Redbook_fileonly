#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

using namespace std;

enum VAO_ID { Triangles, numVAOs };
enum BUFFER_ID { ArrayBuffer, numBuffers };
enum ATTRIB_ID { vPosition = 0 };

GLuint VAOs[numVAOs];
GLuint buffers[numBuffers];
const GLuint numVertices = 3;

static const char* vertexShaderText =
"#version 450 core\n"
"layout ( location = 0 ) in vec4 vPosition;\n"
"void main()\n"
"{\n"
"gl_Position = vPosition;\n"
"}\n";

static const char* fragment_shader_text =
"#version 450 core\n"
"layout ( location = 0 ) out vec4 fColor;\n"
"void main()\n"
"{	fColor = vec4 ( 0.5, 0.1, 0.6, 1.0 );	}\n";


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

	GLuint vertexShader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertexShader, 1, &vertexShaderText, NULL);
	glCompileShader ( vertexShader );

	GLuint fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, &fragment_shader_text, NULL );
	glCompileShader ( fragShader );

	GLuint program = glCreateProgram ();
	glAttachShader ( program, vertexShader );
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
	static const float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv ( GL_COLOR, 0, black );
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