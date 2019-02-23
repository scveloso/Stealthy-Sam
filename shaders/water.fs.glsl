#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform vec2 sam_position;
uniform vec2 text_position;
uniform int text_cond;
uniform vec2 e_position;
uniform int key_cond;


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
    1.0, 0,    0,
    0,  -1.0,  0,
    0,   0,  1.0
    );
	vec3 sp = transform* vec3(sam_position,1);
<<<<<<< HEAD
  vec3 tp= transform* vec3(text_position,1);
  vec3 ep= transform* vec3(e_position,1);
	vec4 in_color = texture(screen_texture, uv.xy);
  float h = 16;
  float w = 150;
  float we = 210;
  float x= gl_FragCoord.x;
  float x0= tp.x;
  float y= gl_FragCoord.y;
  float y0= tp.y;
  float x1= ep.x;
  float y1= ep.y;

	float d= (gl_FragCoord.x - sp.x)*(gl_FragCoord.x - sp.x)+(gl_FragCoord.y- sp.y)*(gl_FragCoord.y-sp.y);

	if (d <= (100*100))
	{
		color= (in_color);
  }
	 else if (x <= x0+w && x >= x0-w && y <= y0+h && y >= y0-h && text_cond == 1){
    color= (in_color);
  }
  else if (x <= x1+we && x >= x1-we && y <= y1+h && y >= y1-h && key_cond == 1){
   color= (in_color);
 }
  else {
    color= vec4(0,0,0,1);
=======
	vec4 in_color = texture(screen_texture, uv.xy);

	float d= (gl_FragCoord.x - sp.x)*(gl_FragCoord.x - sp.x)+(gl_FragCoord.y- sp.y)*(gl_FragCoord.y-sp.y);

	if (d > (100*100))
	{
		color= vec4(0,0,0,1);
	} else {
		color= (in_color);
>>>>>>> e06218ff58fb4ad84070409b7f668472c84ef270
	}
}
