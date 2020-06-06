#version 450 core

uniform myUniform 
{
	float red;
	float green;
	float blue;
	float movement;
};

layout ( location = 0 ) in vec4 vPosition;

void
main () 
{
	gl_Position = vPosition + vec4 ( vec2 ( movement ), 0.0, 0.0 );
}