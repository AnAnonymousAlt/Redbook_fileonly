#ifndef _GL_DEF

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>

#define _GL_DEF
#endif // !_GL_DEF

#include <helper_functions.h>
#include <cstdio>
void
init ()
{
	GLuint program;

	glClearColor ( 0.0, 0.5, 0.0, 1.0 );

	loadShader ( program );

	// Initialize uniform values in uniform block "myUniform"
	GLuint uboIndex;
	GLint uboSize;
	GLvoid *uboBuffer;

	// Find the uniform buffer index for "Uniforms", and
	// determine the block's sizes
	uboIndex = glGetUniformBlockIndex ( program, "myUniform" );
	glGetActiveUniformBlockiv ( program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize );

	uboBuffer = malloc ( uboSize );

	// Debug
	if ( uboBuffer == NULL )
	{
		fprintf ( stderr, "Unable to allocate buffer\n" );
		exit ( EXIT_FAILURE );
	} else
	{
		enum { Red, Green, Blue, Movement, NumUniforms };

		/* Values to be stored in the buffer object */
		GLfloat red = 0.1;
		GLfloat green = 0.7;
		GLfloat blue = 0.1;
		GLfloat movement = 1.0;

		/* Since we know the names of the uniforms
		** in our block, make an array of those values' name */
		// asedfasdfasdf
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
		glGetActiveUniformsiv ( program, NumUniforms, indices, GL_UNIFORM_SIZE, size );
		glGetActiveUniformsiv ( program, NumUniforms, indices, GL_UNIFORM_OFFSET, offset );
		glGetActiveUniformsiv ( program, NumUniforms, indices, GL_UNIFORM_TYPE, type );

		/* copy the uniform values into the buffer */
		memcpy ( uboBuffer + offset)


	}
	
}

int
main ( int argc, char **argv )
{
	return 0;
}

