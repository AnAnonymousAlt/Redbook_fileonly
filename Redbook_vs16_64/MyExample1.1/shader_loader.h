// My shader loader function
#pragma once

#ifndef _GL_DEF
#include <GLFW/glfw3.h>
#include <GL/gl3w.h>
#define _GL_DEF
#endif // !_GL_DEF

#include <fstream>
#include <string>



inline void loadShader ( GLuint program )
{
	// Read vertex shader file
	std::ifstream inv ( "vertex.glsl" );
	std::string vertShaderString = std::string (
		std::istreambuf_iterator<char> ( inv ),
		std::istreambuf_iterator<char> () );
	const char *vertShaderText = vertShaderString.c_str ();

	// Read fragment shader file
	std::ifstream inf ( "fragment.glsl" );
	std::string fragShaderString = std::string (
		std::istreambuf_iterator<char> ( inf ),
		std::istreambuf_iterator<char> () );
	const char *fragShaderText = fragShaderString.c_str ();

	// Create a shader object
	GLuint vertShader = glCreateShader ( GL_VERTEX_SHADER );
	// Replace the source code in a shader object
	// 1 is the number of string array at argument 3
	// NULL is the array of string length
	glShaderSource ( vertShader, 1, &vertShaderText, NULL );
	// Compile a shader object
	glCompileShader ( vertShader );

	// Same thing as the vertex shader
	GLuint fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, &fragShaderText, NULL );
	glCompileShader ( fragShader );

	// Attaches a shader object to a program object
	// Shaders that are to be linked together in a program object must first be attached to that program object.
	// This action of attaching indicates that shader will be included in link operations that will be performed on program.
	glAttachShader ( program, vertShader );
	glAttachShader ( program, fragShader );
	// Links a program object
	// As a result of a successful link operation, all active user-defined uniform variables belonging to program will be initialized to 0.
	// And each of the program object's active uniform variables will e assigned a location that can be queried by calling glGetUniformLocation.
	glLinkProgram ( program );
	// Install a program object as part of current rendering state
	// Any shader objects of type GL_VERTEX_SHADER, GL_GEOMETRY_SHADER and GL_FRAGMENT_SHADER attached to program are used to create excutable that run on the corresbonding programmable processor.
	glUseProgram ( program );
}