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
	program);
}