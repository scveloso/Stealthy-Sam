#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform vec2 sam_position;

in vec2 uv;

layout(location = 0) out vec4 color;

float circle(vec2 st, float radius){
  vec2 dist = st-vec2(0.5);
	return 1.-smoothstep(radius-(radius*0.01),
                         radius+(radius*0.01),
                         dot(dist,dist)*4.0);
}

float dimmer(float d) {
	
	if (pow(d/1000, -0.1) > 1){
		return 1.0;
	} else {
		return pow(d/1000,-0.3);
	}
}

void main()
{
  mat3 transform= mat3(
    1.0, 0,    0,
    0,  -1.0,  0,
    0,   0,  1.0
    );
	vec3 sp = transform* vec3(sam_position,1);
	vec4 in_color = texture(screen_texture, uv.xy);

	float d = (gl_FragCoord.x - sp.x)*(gl_FragCoord.x - sp.x)+(gl_FragCoord.y- sp.y)*(gl_FragCoord.y-sp.y);



	float p = dimmer(d);
	color = in_color*p;

	if (time < 5*10){
		color = color*(time/50);
	}
}

