

#include <helper_functions.h>
// to eliminate error
void func (){
	GLint materialShaderLoc;
	GLuint ambientIndex;
	GLuint diffuseIndex;

	// Eliminate error
	GLuint program;
	glUseProgram ( program );

	materialShaderLoc = glGetSubroutineUniformLocation (
	program, GL_VERTEX_SHADER, "materialShader");

	if ( materialShaderLoc < 0 )
	{
		// Error: materialShader is not an active subroutine
		// uniform in the shader
	}

	ambientIndex = glGetSubroutineIndex ( program,
										  GL_VERTEX_SHADER,
										  "ambient" );
	diffuseIndex = glGetSubroutineIndex ( program,
										  GL_VERTEX_SHADER,
										  "diffuse" );

	if ( ambientIndex == GL_INVALID_INDEX ||
		 diffuseIndex == GL_INVALID_INDEX )
	{
		// Error: the specified subroutiens are not active 
		// in the currently bound program for teh GL_VERTEX_SHADER
		// stage.
	}
	else
	{
		GLsizei n;
		glGetIntegerv ( GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &n );

		GLuint *indices = new GLuint[n];
		indices[materialShaderLoc] = ambientIndex;

		glUniformSubroutinesuiv ( GL_VERTEX_SHADER, n, indices );

		delete[] indices;

	}
}