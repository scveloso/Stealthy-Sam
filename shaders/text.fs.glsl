#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform int text_cond;
uniform int key_cond;
uniform int death_cond;
uniform int re_cond;

in vec2 uv;
in vec2 t_position;
in vec2 keye_position;
in vec2 re_position;


layout(location = 0) out vec4 color;

  vec3 tp=  vec3(t_position,1);
  vec3 ep=  vec3(keye_position,1);
  vec3 rp=  vec3(re_position,1);
	vec4 in_color = texture(screen_texture, uv.xy);


  float h = 15;
  float w = 155;
  float we = 240;
  float wr= 200;
  float x= gl_FragCoord.x;
  float y= gl_FragCoord.y;
  float x0= tp.x;
  float y0= tp.y;
  float x1= ep.x;
  float y1= ep.y;
  float x2= rp.x;
  float y2= rp.y;

  void main()
  {
    if (death_cond ==0){
      if (x <= x0+w && x >= x0-w && y <= y0+h && y >= y0-h && text_cond == 1){
        color= (in_color);
      }
      else if (x <= x1+we && x >= x1-we && y <= y1+h && y >= y1-h && key_cond == 1){
        color= (in_color);
      }else{
        color= vec4(0,0,0,0.1)*in_color;
      }
    }

    else if (death_cond == 1) {
      if (x <= x2+wr+7 && x >= x2-wr-7 && y <= y2+h && y >= y2-h && re_cond == 0){
        color= in_color;
      }
      else{
        color= vec4(0,0,0,0.1)*in_color;
      }
    }
  }
