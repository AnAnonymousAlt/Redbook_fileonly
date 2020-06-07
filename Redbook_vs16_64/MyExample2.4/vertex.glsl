#version 450 core


// Use alphabet to sort.
uniform myUniform 
{
	float blue;
	// Here I don't know why but "enable" cannot be boolean
	bool enabled;
	float green;
	float movement;
	float red;
};

layout ( location = 0 ) in vec4 vPosition;

void
main () 
{
	bool is_enabled = enabled;
	vec4 ( red, green, blue, 0.0);
	vec4 backward = vec4 ( -0.4, -0.4, 0.0, 0.0 );
	if (is_enabled) {
		gl_Position = vPosition + vec4 ( vec2 ( movement ), 0.0, 0.0 );
	}
	else {
		gl_Position = vPosition + backward;
	}
}