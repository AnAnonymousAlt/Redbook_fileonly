#version 450 core

uniform myUniform 
{
	float red;
	float green;
	float blue;
	float movement;
	bool enable;
};

layout ( location = 0 ) out vec4 fColor;

void
main () 
{
	if ( enable ) 
	{
		fColor = vec4 ( red, green, blue, 1.0 );
	} 
	else 
	{
		fColor = vec4 ( 0.0, 0.0, 1.0, 1.0 );
	}
}