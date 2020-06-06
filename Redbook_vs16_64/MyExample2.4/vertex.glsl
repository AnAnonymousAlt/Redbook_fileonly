#version 450 core

uniform myUniform 
{
	float red;
	float green;
	float blue;
	float movement;
	bool enable;
};

layout ( location = 0 ) in vec4 vPosition;

void
main () 
{
	if (enable) {
		gl_Position = vPosition + vec4 ( vec2 ( movement ), 0.0, 0.0 );
	}
	else {
		gl_Position = vPosition + vec4 ( -0.4, -0.4, 0.0, 0.0 );
	}
}