#ifndef _GL_DEF

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>

#define _GL_DEF
#endif // !_GL_DEF

#include <helper_functions.h>
#include <cstdio>

enum VAO_ID
{
	Triangles, numVAOs
};

enum Buffer_ID
{
	ArrayBuffer, numBuffers
};

enum ATTRIB_ID
{
	vPosition = 0
};

GLuint VAOs[numVAOs];
GLuint buffers[numBuffers];

const GLint numVertices = 3;

void
init ()
{
	const GLfloat vertices[numVertices][2] =
	{
		{ -0.5, -0.5 },
		{  0.0, -0.5 },
		{ -0.5,  0.0 }
	};

	glCreateVertexArrays ( numVAOs, VAOs );
	glCreateBuffers ( numBuffers, buffers );
	glNamedBufferData ( buffers[ArrayBuffer], sizeof ( vertices ),
						&vertices, 0 );

	GLuint program;
	loadShader ( program );

	// Initialize uniform values in uniform block "myUniform"
	GLuint uboIndex;
	GLint uboSize;
	GLvoid *uboBuffer;
	GLuint ubo;

	// Find the uniform buffer index for "Uniforms", and
	// determine the block's sizes
	uboIndex = glGetUniformBlockIndex ( program, "myUniform" );
	glGetActiveUniformBlockiv ( program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize );

	uboBuffer = malloc ( uboSize );

	if ( uboBuffer == NULL )
	{
		fprintf ( stderr, "Unable to allocate buffer\n" );
		exit ( EXIT_FAILURE );
	} else
	{
		enum
		{
			Red, Green, Blue, Movement, NumUniforms
		};

		/* Values to be stored in the buffer object */
		GLfloat red = 0.1;
		GLfloat green = 0.7;
		GLfloat blue = 0.1;
		GLfloat movement = 1.0;

		/* Since we know the names of the uniforms
		** in our block, make an array of those values' name */
		const char *names[NumUniforms] =
		{
			"red",
			"green",
			"red",
			"movement"
		};
		/* Query the necessary attributes to determine
		** where in the buffer we should write
		** the values */
		GLuint indices[NumUniforms];
		GLint size[NumUniforms];
		GLint offset[NumUniforms];
		GLint type[NumUniforms];

		glGetUniformIndices ( program, NumUniforms, names, indices );
		glGetActiveUniformsiv ( program, NumUniforms, indices,
								GL_UNIFORM_SIZE, size );
		glGetActiveUniformsiv ( program, NumUniforms, indices,
								GL_UNIFORM_OFFSET, offset );
		glGetActiveUniformsiv ( program, NumUniforms, indices,
								GL_UNIFORM_TYPE, type );

		/* copy the uniform values into the buffer */
		memcpy ( ( char * ) uboBuffer + offset[Red], &red,
				 size[Red] * typeSize ( type[Red] ) );
		memcpy ( ( char * ) uboBuffer + offset[Green], &green,
				 size[Green] * typeSize ( type[Green] ) );
		memcpy ( ( char * ) uboBuffer + offset[Blue], &blue,
				 size[Blue] * typeSize ( type[Blue] ) );
		memcpy ( ( char * ) uboBuffer + offset[Movement], &movement,
				 size[Movement] * typeSize ( type[Movement] ) );

		/* Create the uniform buffer object,
		** initialize its storage, and associated
		** it with the shader program */
		glGenBuffers ( 1, &ubo );
		glBindBuffer ( GL_UNIFORM_BUFFER, ubo );
		glBufferData ( GL_UNIFORM_BUFFER, uboSize, uboBuffer,
					   GL_STATIC_DRAW );
		glBindBufferBase ( GL_UNIFORM_BUFFER, uboIndex, ubo );
	}

	glBindVertexArray ( VAOs[Triangles] );
	glBindBuffer ( GL_ARRAY_BUFFER, buffers[ArrayBuffer] );
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
								 ( void * ) 0 );
	glEnableVertexAttribArray ( vPosition );
}

void
display ()
{
	glClearColor ( 0.0, 0.0, 0.1, 1.0 );

	glDrawArrays ( GL_TRIANGLES, 0, numVertices );
}

int
main ( int argc, char **argv )
{
	glfwInit ();

	GLFWwindow *window = glfwCreateWindow ( 640, 480, "Triangle", NULL, NULL );

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

