#pragma once

#ifndef _GL_DEF
#include <GLFW/glfw3.h>
#include <GL/gl3w.h>
#define _GL_DEF
#endif // !_GL_DEF

#include <fstream>
#include <string>



/// <summary>
/// my shader loader
/// </summary>
/// <param name="program">the shader program</param>
inline void loadShader ( GLuint program )
{
	// Read vertex shader source code
	std::ifstream inv ( "vertex.glsl" );
	std::string vertShaderString = std::string (
		std::istreambuf_iterator<char> ( inv ),
		std::istreambuf_iterator<char> () );
	const char *vertShaderText = vertShaderString.c_str ();

	// Read fragment shader source code 
	std::ifstream inf ( "fragment.glsl" );
	std::string fragShaderString = std::string (
		std::istreambuf_iterator<char> ( inf ),
		std::istreambuf_iterator<char> () );
	const char *fragShaderText = fragShaderString.c_str ();

	// glIsShader (GLuint)
	// glIsProgram (GLuint)

	// Create a shader object
	GLuint vertShader = glCreateShader ( GL_VERTEX_SHADER );
	// Associate the source code of the shader with the object
	// 1 is the number of string array at argument 3
	// NULL is the array of string length
	glShaderSource ( vertShader, 1, &vertShaderText, NULL );
	// Compile a shader object's source
	glCompileShader ( vertShader );

	// Determine if the compilation finished successfully
	// Return value of compilation status
	GLint vertStatus;
	// Retrieve compilation status
	glGetShaderiv ( vertShader, GL_COMPILE_STATUS, &vertStatus );
	// If compilation failed
	if ( vertStatus == GL_FALSE )
	{
		// Error log length
		GLint logLength;
		// query error log length
		glGetShaderiv ( vertShader, GL_INFO_LOG_LENGTH, &logLength );
		// Error log string
		char *errorLog = ( char * ) malloc ( ( size_t ) logLength );
		// Query error log
		glGetShaderInfoLog ( vertShader, logLength, NULL, errorLog );
		// Print error log
		printf ( "%s\n", errorLog );
		// Exit with error code
		exit ( EXIT_FAILURE );
	}

	// Same thing as the vertex shader
	GLuint fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, &fragShaderText, NULL );
	glCompileShader ( fragShader );

	GLint fragStatus;
	glGetShaderiv ( fragShader, GL_COMPILE_STATUS, &fragStatus );
	if ( fragStatus == GL_FALSE )
	{
		GLint logLength;
		glGetShaderiv ( fragShader, GL_INFO_LOG_LENGTH, &logLength );
		char *errorLog = ( char * ) malloc ( ( size_t ) logLength );
		glGetShaderInfoLog ( fragShader, logLength, NULL, errorLog );
		printf ( "%s\n", errorLog );
		exit ( EXIT_FAILURE );
	}

	// Attaches a shader object to a program object
	// Shaders that are to be linked together in a program object must first be attached to that program object.
	// This action of attaching indicates that shader will be included in link operations that will be performed on program.
	glAttachShader ( program, vertShader );
	glAttachShader ( program, fragShader );
	// Links a program object
	// As a result of a successful link operation, all active user-defined uniform variables belonging to program will be initialized to 0.
	// And each of the program object's active uniform variables will e assigned a location that can be queried by calling glGetUniformLocation.
	glLinkProgram ( program );
	// Link error checking
	GLint linkStatus = GL_FALSE;
	glGetProgramiv ( program, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_FALSE )
	{
		GLint logLength;
		glGetProgramiv ( program, GL_INFO_LOG_LENGTH, &logLength );
		char *errorLog = ( char * ) malloc ( ( size_t ) logLength );
		glGetProgramInfoLog ( program, logLength, NULL, errorLog );
		printf ( "%s\n", errorLog );
		exit ( EXIT_FAILURE );
	}

	// Install a program object as part of current rendering state
	// Any shader objects of type GL_VERTEX_SHADER, GL_GEOMETRY_SHADER and GL_FRAGMENT_SHADER attached to program are used to create excutable that run on the corresbonding programmable processor.
	glUseProgram ( program );

	// Delete shaders
	glDeleteShader ( vertShader );
	glDeleteShader ( fragShader );
	// glDeleteProgram ( program );
}



/// <summary>
/// Return size of the GL type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
size_t
typeSize ( GLenum type )
{
	size_t size;
	#define CASE(Enum, Count, Type) \
	case Enum: size = Count * sizeof (Type); break

	switch ( type )
	{
		CASE ( GL_FLOAT, 1, GLfloat );
		CASE ( GL_FLOAT_VEC2, 2, GLfloat );
		CASE ( GL_FLOAT_VEC3, 3, GLfloat );
		CASE ( GL_FLOAT_VEC4, 4, GLfloat );
		CASE ( GL_INT, 1, GLint );
		CASE ( GL_INT_VEC2, 2, GLint );
		CASE ( GL_INT_VEC3, 3, GLint );
		CASE ( GL_INT_VEC4, 4, GLint );
		CASE ( GL_UNSIGNED_INT, 1, GLuint );
		CASE ( GL_UNSIGNED_INT_VEC2, 2, GLuint );
		CASE ( GL_UNSIGNED_INT_VEC3, 3, GLuint );
		CASE ( GL_UNSIGNED_INT_VEC4, 4, GLuint );
		CASE ( GL_BOOL, 1, GLboolean );
		CASE ( GL_BOOL_VEC2, 2, GLboolean );
		CASE ( GL_BOOL_VEC3, 3, GLboolean );
		CASE ( GL_BOOL_VEC4, 4, GLboolean );
		CASE ( GL_FLOAT_MAT2, 4, GLfloat );
		CASE ( GL_FLOAT_MAT2x3, 6, GLfloat );
		CASE ( GL_FLOAT_MAT2x4, 8, GLfloat );
		CASE ( GL_FLOAT_MAT3, 9, GLfloat );
		CASE ( GL_FLOAT_MAT3x2, 6, GLfloat );
		CASE ( GL_FLOAT_MAT3x4, 12, GLfloat );
		CASE ( GL_FLOAT_MAT4, 16, GLfloat );
		CASE ( GL_FLOAT_MAT4x2, 8, GLfloat );
		CASE ( GL_FLOAT_MAT4x3, 12, GLfloat );
	default:
		fprintf ( stderr, "Unknown type: -x%x\n", type );
		#undef CASE
		exit ( EXIT_FAILURE );
		break;
	}
	#undef CASE
	return size;
}