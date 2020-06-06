#ifndef _GL_DEF

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

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
	ArrayBuffer, UniformBuffer, numBuffers
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
	glClearColor ( 0.0f, 0.0f, 0.1f, 1.0f );

	const GLfloat vertices[numVertices][2] =
	{
		{ -0.5f, -0.5f },
		{  0.0f, -0.5f },
		{ -0.5f,  0.0f }
	};

	glCreateVertexArrays ( numVAOs, VAOs );
	glCreateBuffers ( numBuffers, buffers );


	GLuint program = glCreateProgram ();
	loadShader ( program );
	
	GLvoid *uboBuffer;


	// Allocate uniform buffer
	{
		// Initialize uniform values in uniform block "myUniform"
		GLuint uboIndex;
		GLint uboSize;

		// Find the uniform buffer index for "Uniforms", and
		// determine the block's sizes
		uboIndex = glGetUniformBlockIndex ( program, "myUniform" );
		glGetActiveUniformBlockiv ( program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize );

		uboBuffer = malloc ( uboSize );

		if ( uboBuffer == NULL )
		{
			fprintf ( stderr, "Unable to allocate buffer\n" );
			exit ( EXIT_FAILURE );
		}

		enum
		{
			Red, Green, Blue, Movement, Enable, NumUniforms
		};

		/* Values to be stored in the buffer object */
		GLfloat red = 0.1f;
		GLfloat green = 0.7f;
		GLfloat blue = 0.1f;
		GLfloat movement = 1.0f;
		GLboolean enable = TRUE;

		/* Since we know the names of the uniforms
		** in our block, make an array of those values' name */
		const char *names[NumUniforms] =
		{
			"red",
			"green",
			"red",
			"movement",
			"enable"
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
		memcpy ( ( char * ) uboBuffer + offset[Enable], &enable,
				 size[Enable] * typeSize ( type[Enable] ) );


		/* Create the uniform buffer object,
		** initialize its storage, and associated
		** it with the shader program */
		glNamedBufferStorage ( buffers[UniformBuffer], uboSize, uboBuffer, 0 );
		glBindBuffer ( GL_UNIFORM_BUFFER, buffers[UniformBuffer] );
		glBindBufferBase ( GL_UNIFORM_BUFFER, uboIndex, buffers[UniformBuffer] );
	}

	glNamedBufferStorage ( buffers[ArrayBuffer], sizeof ( vertices ),
						   &vertices, 0 );
	glBindVertexArray ( VAOs[Triangles] );
	glBindBuffer ( GL_ARRAY_BUFFER, buffers[ArrayBuffer] );
	glVertexAttribPointer ( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
							( void * ) 0 );
	glEnableVertexAttribArray ( vPosition );
}

void
display ()
{
	glClear ( GL_COLOR_BUFFER_BIT );
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

