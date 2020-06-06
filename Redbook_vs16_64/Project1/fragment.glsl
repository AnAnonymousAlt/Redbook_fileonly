#version 450 core

uniform myUniform 
{
	float red;
	float green;
	float blue;
	float movement;
};

layout ( location = 0 ) out vec4 fColor;

void
main () 
{
	fColor = vec4 ( red, green, blue, 1.0 );
}