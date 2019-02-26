#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform vec2 sam_position;
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

  // float displace= 0;
  //
  // bool inside_tri(float p1p2, float p1p3, float p2p3){
  //   if (p1p3 > 0 && p1p3 == p1p2){
  //     return false;
  //   }
  //   if (p2p3 > 0 && p2p3 != p1p2){
  //     return false;
  //   }else{
  //     return true;
  //   }
  // }

void main()
{

  // if (enemy_direction == 5){
  //   displace= -4;
  // }
  // if (enemy_direction == 7){
  //   displace= 4;
  // }
  //
  // vec2 p1= e.xy + vec2(displace, 0);
  // vec2 p2= p1 + vec2(s, 0);
  // vec2 p3= p2 + vec2(0, t);
  // float p1_x= x- p1.x;
  // float p1_y= y-p1.y;
  // float p1p2= (p2.x-p1.x)*p1_x - (p2.y-p1.y)*p1.y;
  // float p1p3= (p3.x-p1.x)*p1_y - (p3.y-p1.y)*p1_y;
  // float p2p3= (p3.x-p2.x)*(y-p2.y)-(p3.y-p2.y)*(x-p2.x);
  // int i=0;
  // for(const auto& enemy_direct: enemy_position){
  //   vec2 temp= transform * vec3(enemy_direct,1);
  //   e[i]= temp.xy;
  // }
  // for (const auto& ed: e){
  //   float de= (x - ed.x)*(x - ed.x)+(y- ed.y)*(y-ed.y);
  //   if (de <= (50*50)){
  //     color=(in_color);
  //   }
  // }

	float d= (x - sp.x)*(x - sp.x)+(y- sp.y)*(y-sp.y);

  float p = dimmer(d);

//   if (d <= 100*100){
//   color= in_color;
// }
if (death_cond ==0){
 if (x <= x0+w && x >= x0-w && y <= y0+h && y >= y0-h && text_cond == 0){
   color= (in_color);
 }
else if (x <= x1+we && x >= x1-we && y <= y1+h && y >= y1-h && key_cond == 0){
  color= (in_color);
}
// else if (x <= x2+we && x >= x2-we && y <= y2+h && y >= y2-h && re_cond == 0){
//   color= (in_color);
// }
else if (time < 5*10){
        color = in_color*p*(time/50);
 }
 else{
        color= (in_color*p);
    }
}
else if (death_cond == 1) {
  color= vec4(1,0,0,0.5);
  //if (x <= x2+we && x >= x2-we && y <= y2+h && y >= y2-h && re_cond == 0){
//color= in_color;
//}
}

}
