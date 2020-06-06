#ifndef _GL_DEF
#include <GLFW/glfw3.h>
#include <GL/gl3w.h>
#define _GL_DEF
#endif // !_GL_DEF

#include <iostream>
#include <cstdio>
#include <cstdlib>


using namespace std;

// Vertex and fragment shaders that share a block of uniforms
// named "Uniforms"
const char *vertShaderText =
{
	"#version 330 core\n"
	"uniform Uniforms {"
	"  vec3 translation;"
	"  float scale;"
	"  vec4 rotation;"
	"  bool enabled;"
	"};"
	"in vec2 vPos;"
	"in vec3 vColor;"
	"out vec4 fColor;"
	"void main()"
	"{"
	"  vec3 pos = vec3 (vPos, 0.0);"
	"  float angle = radians(rotation[0]);"
	"  vec3 axis = normalize (rotation.yzw);"
	"  mat3 I = mat3(1.0);"
	"  mat3 S = mat3 ( 0, -axis.z, axis.y,"
	"  axis.z, 0, -axis.x,"
	"  -axis.y, axis.x, 0);"
	"  mat3 uut = outerProduct(axis, axis);"
	"  mat3 rot = uuT + cos(angle)*(I-uuT)+sin(angle)*S;"
	"  pos *= scale;"
	"  pos *= rot;"
	"  pos += translation;"
	"  fColor = vec4(scale, scale, scale, 1);"
	"  gl_Position = vec4(pos, 1);"
	"}"
};
const char *fragShaderText = {
	"#version 330 core\n"
	"uniform Uniforms {"
	"  vec3 translation;"
	"  float scale;"
	"  vec4 rotation;"
	"  bool enabled;"
	"};"
	"in vec4 fColor;"
	"out vec4 color;"
	"void main()"
	"{"
	"  color = fColor;"
	"}"
};

// Helper function to convert GLSL types to storage sizes
size_t
TypeSize ( GLenum type )
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
		#undef CASE
	default:
		fprintf ( stderr, "Unknown type: -x%x\n", type );
		exit ( EXIT_FAILURE );
		break;
	}
	return size;
}

void
init ()
{
	GLuint program;

	glClearColor ( 1, 0, 0, 1 );

	GLuint vertShader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertShader, 1, &vertShaderText, NULL );
	glCompileShader ( vertShader );
	GLuint fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, &fragShaderText, NULL );
	glCompileShader ( fragShader );

	GLuint program = glCreateProgram ();
	glAttachShader ( program, vertShader );
	glAttachShader ( program, fragShader );
	glLinkProgram ( program );
	glUseProgram ( program );

	/* Initialize uniform values in uniform block "Uniforms" */
	GLuint uboIndex;
	GLint uboSize;
	GLuint ubo;
	GLvoid *buffer;

	// Find the uniform buffer index for "Uniforms", and
	// determine the block's sizes
	uboIndex = glGetUniformBlockIndex ( program, "Uniforms" );

	glGetActiveUniformBlockiv ( program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize );

	buffer = malloc ( uboSize );

	if ( buffer == NULL )
	{
		fprintf ( stderr, "Unable to allocate buffer\n" );
		exit ( EXIT_FAILURE );
	} else
	{
		enum
		{
			Translation, Scale, Rotation, Enabled, NumUniforms
		};

		/* Values to be stored in the buffer object */
		GLfloat scale = 0.5;
		GLfloat translation[] = { 0.1,0.1,0.1 };
		GLfloat rotation[] = { 90, 0.0,0.0,1.0 };
		GLboolean enabled = GL_TRUE;

		/* Since we know the names of the uniforms in our block,
		** make an array of those values */
		const char *names[NumUniforms] = {
			"translation",
			"scale",
			"rotation",
			"enabled"
		};

		/* Query the necessary attributes to determine where \
		** in the buffer we should write the values */
		GLuint indices[NumUniforms];
		GLint size[NumUniforms];
		GLint offset[NumUniforms];
		GLint type[NumUniforms];

		glGetUniformIndices ( program, NumUniforms, names, indices );
		glGetActiveUniformsiv ( program, NumUniforms, indices,
								GL_UNIFORM_OFFSET, offset );
		glGetActiveUniformsiv ( program, NumUniforms, indices,
								GL_UNIFORM_SIZE, size );
		glGetActiveUniformsiv ( program, NumUniforms, indices,
								GL_UNIFORM_TYPE, type );

		/* Copy the uniform values in to the buffer */
		memcpy ( ( char * ) buffer + offset[Scale], &scale, size[Scale] * TypeSize ( type[Scale] ) );
		memcpy ( ( char * ) buffer + offset[Translation], &translation,
				 size[Translation] * TypeSize ( type[Rotation] ) );
		memcpy ( ( char * ) buffer + offset[Rotation], &rotation, size[Rotation] * TypeSize ( type[Scale] ) );
		memcpy ( ( char * ) buffer + offset[Enabled], &enabled, size[Enabled] * TypeSize ( type[Enabled] ) );

		/* Create the uniform buffer object,
		** initialize its storage, and associated
		** it with the shader program */
		glGenBuffers ( 1, &ubo );
		glBindBuffer ( GL_UNIFORM_BUFFER, ubo );
		glBufferData ( GL_UNIFORM_BUFFER, uboSize, buffer, GL_STATIC_DRAW );

		glBindBufferBase ( GL_UNIFORM_BUFFER, uboIndex, ubo );
	}
	// ...
}
// ...