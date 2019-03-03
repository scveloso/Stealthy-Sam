#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform vec2 circle_light_position;
uniform vec2 text_position;
uniform int text_cond;
uniform vec2 e_position;
uniform int key_cond;
uniform int death_cond;
uniform int re_cond;
uniform vec2 r_position;

// uniform vec2 enemy_position;
// uniform int enemy_direction;


in vec2 uv;
in vec2 s_position;
//in float d;

layout(location = 0) out vec4 color;
//layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;

mat3 transform= mat3(
  1.0, 0,    0,
  0,  -1.0,  0,
  0,   0,  1.0
  );
  vec3 sp = vec3(s_position,1);
  vec3 tp= transform* vec3(text_position,1);
  vec3 ep= transform* vec3(e_position,1);
  vec3 rp= transform* vec3(r_position,1);
  //vec3 e= transform* vec3(enemy_position,1);
  //vec2 e[];
	vec4 in_color = texture(screen_texture, uv.xy);


  float h = 16;
  float w = 150;
  float we = 240;
  // float x= gl_FragCoord.x;
  // float y= 800.0-gl_FragCoord.y;
  float x= gl_FragCoord.x;
  float y= gl_FragCoord.y;
  float x0= tp.x;
  float y0= tp.y;
  float x1= ep.x;
  float y1= ep.y;
  float x2= ep.x;
  float y2= ep.y;
  float s= 10;
  float t=5;

float dimmer(float d) {

	if (pow(d/1000, -0.1) > 1){
		return 1.0;
	} else {
		return pow(d/1000,-0.3);
	}
}

void main()
{

    float d= (x - sp.x)*(x - sp.x)+(y- sp.y)*(y-sp.y);

    float p = dimmer(d);

    if (death_cond ==0){
        if (x <= x0+w && x >= x0-w && y <= y0+h && y >= y0-h && text_cond == 1){
            color= (in_color);
        }
        else if (x <= x1+we && x >= x1-we && y <= y1+h && y >= y1-h && key_cond == 1){
        color= (in_color);
        }
        else if (time < 5*10){
            color = in_color*p*(time/50);
        }
        else{
            color= (in_color*p);
        }
    }

    else if (death_cond == 1) {
        color= vec4(1,0,0,0.5);
    }

    // uncomment if you want to turn off the shader
    //color= in_color;
}
