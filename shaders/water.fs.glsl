#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;

in vec2 uv;

layout(location = 0) out vec4 color;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// HANDLE THE WATER WAVE DISTORTION HERE (you may want to try sin/cos)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
vec2 distort(vec2 uv) {
	// a affects the amount the x travel affects, b is the max amplitude (height of difference),
	// c is the time factor (greater c, faster change happens)
	//                 a      b                  c
	// uv.x += sin(uv.y * 20.) * 0.003 * sin(time * 0.3);
	// uv.y += sin(uv.x * 20.) * 0.003 * sin(time * 0.3);
 	return uv;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// HANDLE THE COLOR SHIFTING HERE (you may want to make the image slightly blue-ish)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
vec4 color_shift(vec4 in_color) {
	vec4 color = in_color;
	// change the r and g values so blue is more prominent
	color[0] *= 0.9;
	color[1] *= 0.9;
	return color;
}

vec4 fade_color(vec4 in_color) {
	vec4 color = in_color;
	if (dead_timer > 0)
		color -= 0.1 * dead_timer * vec4(0.1, 0.1, 0.1, 0);

	return color;
}

void main()
{
	vec2 coord = distort(uv);

    vec4 in_color = texture(screen_texture, coord);
    color = color_shift(in_color);
    color = fade_color(color);
}
