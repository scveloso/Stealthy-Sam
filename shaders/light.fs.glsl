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
	if (pow(d/2000, -0.1) > 1){
		return 1;
	} else {
		return pow(d/2000,-0.3);
	}
}



void main()
{
    float d= (x - sp.x)*(x - sp.x)+(y- sp.y)*(y-sp.y);
    //create circle of light around each torch;
    for (int i = 0; i < 5; i++){
        d_torch[i]=(x- tor_position[i].x)*(x- tor_position[i].x)+ (y-tor_position[i].y)*(y-tor_position[i].y);
    }

    int r = 150;
		bool dt[5];
    dt[1] = d_torch[1] <= r*r;
    dt[2] = d_torch[2] <= r*r;
    dt[3] = d_torch[3] <= r*r;
    dt[4] = d_torch[4] <= r*r;
    dt[0] = d_torch[0] <= r*r;
		float k[5];
		float p= dimmer(d);

		if (death_cond ==0){
			if (dt[1] || dt[2] || dt[3] || dt[4] || dt[0] )
			{
				k[1] = dimmer(d_torch[1]);
				k[2] = dimmer(d_torch[2]);
				k[3] = dimmer(d_torch[3]);
				k[4] = dimmer(d_torch[4]);
				k[0] = dimmer(d_torch[0]);

				if(dt[0]){
					if( p > k[0]){
						color=in_color*p;
					}
					else if (dt[1] || dt[2] || dt[3] || dt[4]){
						for(int i=0; i<5; i++){
							if(k[i] <  k[0]){
								color= 0.54*in_color*(9*k[i]+0.8*k[0]);
							}
							else{
								color= in_color*k[0];
							}
						}
					}
						else{
							color= in_color*k[0];
						}
					}

				if(dt[1]){
				  if( p > k[1]){
				    color=in_color*p;
				    }
						else if (dt[0] || dt[2] || dt[3] || dt[4]){
							for(int i=0; i<5; i++){
								if(k[i] <  k[1] ){
									color= 0.54*in_color*(9*k[i]+0.8*k[1]);
								}else{
									color= in_color*k[1];
								}
							}
						}
						else{
				      color= in_color*k[1];
				    }
				  }
				  if(dt[2]){
				    if( p > k[2]){
				      color=in_color*p;
				      }
							else if (dt[0] || dt[1] || dt[3] || dt[4]){
								for(int i=0; i<5; i++){
									if(k[i] < k[2] ){
										color= 0.54*in_color*(9*k[i]+0.8*k[2]);
									}else{
										color= in_color*k[2];
									}
								}
							}
							else{
				        color= in_color*k[2];
				      }
				    }
				    if(dt[3]){
				      if( p > k[3]){
				        color=in_color*p;
				        }
								else if (dt[0] || dt[2] || dt[1] || dt[4]){
									for(int i=0; i<5; i++){
										if(k[i] <  k[3] ){
											color= 0.54*in_color*(9*k[i]+0.8*k[3]);
										}else{
											color= in_color*k[3];
										}
									}
								}
								else{
				          color= in_color*k[3];
				        }
				      }
				      if(dt[4]){
				        if( p > k[4]){
				          color=in_color*p;
				          }
									else if (dt[0] || dt[2] || dt[3] || dt[1]){
										for(int i=0; i<5; i++){
											if(k[i] <  k[4] ){
											 color= 0.54*in_color*(9*k[i]+0.8*k[4]);
											}else{
												color= in_color*k[4];
											}
										}
									}
									else{
				            color= in_color*k[4];
				          }
				        }
			}
				else{
					color= in_color*p;
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
