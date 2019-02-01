#version 330

// From Vertex Shader
in vec3 vcolor;
in vec2 vpos; // Distance from local origin

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform int light_up;
uniform int is_dead;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	color = vec4(fcolor * vcolor, 1.0);

	if (is_dead == 1)
	{
	    color = vec4(vec3(1.0, 0.6, 0.6) * vcolor * fcolor, 1.0);
	}


	// Sam mesh is contained in a 2x2 square (see sam.mesh for vertices)
	float radius = distance(vec2(0.0), vpos);
	if (light_up == 1 && radius < 1.0)
	{
		// 0.6 is just to make it not too strong
		color.xyz += (1.0 - radius) * 0.6 * vec3(1.0, 1.0, 0.6);
	}
}