#version 450 core

uniform myUniform 
{
	float blue;
	bool enabled;
	float green;
	float movement;
	float red;
};

layout ( location = 0 ) out vec4 fColor;

void
main () 
{
	vec4 color_green = vec4 ( 0.0, 0.5, 0.0, 1.0 );

	if ( enabled ) 
	{
		fColor = vec4 ( red, green, blue, 1.0 );
	} 
	else 
	{
		fColor = color_green;
	}
}