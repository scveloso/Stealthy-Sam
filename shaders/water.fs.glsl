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


void main()
{
  mat3 transform= mat3(
    1.0, 0, 0,
    0, -1.0, 0,
    0, 0, 1.0
    );
	vec3 sp= transform* vec3(sam_position,1);
	//vec2 sp=sam_position;
	vec4 in_color = texture(screen_texture, uv.xy);
  //vec3 fcolor = vec3(circle(sp, 0.9));
	//color= vec4(fcolor, 1.0);
	float d= (gl_FragCoord.x - sp.x)*(gl_FragCoord.x - sp.x)+(gl_FragCoord.y- sp.y)*(gl_FragCoord.y-sp.y);
	if (d > (100*100)){
		color= vec4(0,0,0,1);
	}else{
		color= (in_color);
    //color= vec4(1,1,1,1);
	}


}
