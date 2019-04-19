#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform int death_cond;
uniform int tutorial_cond;

in vec2 uv;
in vec2 s_position;
in vec2 tor_position[5];

layout(location = 0) out vec4 color;

vec4 in_color = texture(screen_texture, uv.xy);
float x= gl_FragCoord.x;
float y= gl_FragCoord.y;
vec3 sp = vec3(s_position,1);
float d_torch[5];

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
    //create circle of light around each torch;
    int j=0;
    for (int i = 0; i < 5; i++){
      if (i==j){
        d_torch[j]=(x- tor_position[j].x)*(x- tor_position[j].x)+ (y-tor_position[j].y)*(y-tor_position[j].y);
      }
      j++;
    }

    int r = 150;
    bool dt1 = d_torch[1] <= r*r;
    bool dt2 = d_torch[2] <= r*r;
    bool dt3 = d_torch[3] <= r*r;
    bool dt4 = d_torch[4] <= r*r;
    bool dt0 = d_torch[0] <= r*r;
		// bool dt1 = d_torch[1] > 0;
    // bool dt2 = d_torch[2] > 0;
    // bool dt3 = d_torch[3] > 0;
    // bool dt4 = d_torch[4] > 0;
    // bool dt0 = d_torch[0] > 0;
    float p = dimmer(d);

    if (death_cond ==0){
      if (dt1 || dt2 || dt3 || dt4 || dt0)
      {
        float k;
        if(dt1){k = dimmer(d_torch[1]);}
        if(dt2){k = dimmer(d_torch[2]);}
        if(dt3){k = dimmer(d_torch[3]);}
        if(dt4){k = dimmer(d_torch[4]);}
        if(dt0){k = dimmer(d_torch[0]);}

        if(p > k){
          color= in_color*p;
          }else{
            color= in_color*k;
          }
        }
      else if (time < 5*10){
        color = in_color*p*(time/50);
      }
			else{
				color= (in_color*p);
			}
    }

    else if (death_cond == 1) {
      vec4 i= vec4(1,0,0,0.3);
      if (d <= 100*100){
        color= i*in_color;
      }
			else {
        color= i;
      }

    }
    if (tutorial_cond == 1)
    {
        color = in_color;
    }


}
