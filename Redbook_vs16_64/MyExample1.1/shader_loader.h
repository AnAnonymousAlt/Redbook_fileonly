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
	std::ifstream inv ( "vertex.glsl" );
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
}