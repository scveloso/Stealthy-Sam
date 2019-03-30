#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform float dead_timer;
uniform int text_cond;
uniform int key_cond;
uniform int death_cond;
uniform int re_cond;
uniform vec2 enemy_direction[5];


in vec2 uv;
in vec2 s_position;
in vec2 t_position;
in vec2 keye_position;
in vec2 re_position;
in vec2 en_position[5];
in vec2 tor_position[5];

layout(location = 0) out vec4 color;

  vec3 sp = vec3(s_position,1);
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

  float tri_w= 100;
  float tri_h=25;
  float displace=15;
  float vdisplace = 10;
  // up and down control variable
  float a=0;
  float b=1;
  float s[5];
  float t[5];
  float st[5];
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
    // float dt= (x- tor_position[0].x)*(x- tor_position[0].x)+ (y-tor_position[0].y)*(y-tor_position[0].y);
    float p = dimmer(d);


    int k=0;
    for(int i = 0; i < 5; i++)
    { // calculate which direction the enemy cone should be placed
      float direction= enemy_direction[i].x;
      if (direction == 1.0){
        displace = -(displace);
        tri_w = -tri_w;
      }
      else if (direction == 3.0){
        displace=0;
        vdisplace= 5+vdisplace;
        a=-1;
        b=0;
        float temp = tri_w;
        tri_w= tri_h;
        tri_h= -temp;
      }
      else if (direction == 4.0){
        displace=0;
        vdisplace= -(5+vdisplace);
        a=-1;
        b=0;
        float temp = tri_w;
        tri_w= tri_h;
        tri_h= temp;
      }
      //calculate the 3 vertex points (p0, p1, p2) of the view cone/traingle
      vec2 p0= vec2(en_position[i].x+displace, en_position[i].y+vdisplace);
      vec2 p1= p0+ vec2(tri_w, -tri_h);
      vec2 p2= p1 + vec2(a*tri_w*2,b*tri_h*2);
      float Area = 0.5 *(-p1.y*p2.x + p0.y*(-p1.x + p2.x) + p0.x*(p1.y - p2.y) + p1.x*p2.y);
      if (i == k){
        //use barycentric coordinate's implicit equation to decide if a point is inside the triangle
        s[k] = 1/(2*Area)*(p0.y*p2.x - p0.x*p2.y + (p2.y - p0.y)*x + (p0.x - p2.x)*y);
        t[k] = 1/(2*Area)*(p0.x*p1.y - p0.y*p1.x + (p0.y - p1.y)*x + (p1.x - p0.x)*y);
        st[k]= 1-s[k]-t[k];
      }
      k++;
    }
    //setting up the checking condition for each of the enemy cone/triangle
    bool st1 = s[1] > 0 && t[1] > 0 && st[1] > 0;
    bool st2 = s[2] > 0 && t[2] > 0 && st[2] > 0;
    bool st3 = s[3] > 0 && t[3] > 0 && st[3] > 0;
    bool st4 = s[4] > 0 && t[4] > 0 && st[4] > 0;
    bool st0 = s[0] > 0 && t[0] > 0 && st[0] > 0;


    if (death_cond ==0){
      if (st1 || st2 || st3 || st4 || st0){
        color= in_color;
      }
      else if (dt1 || dt2 || dt3 || dt4 || dt0){

		float k;
		if(dt1){k = dimmer(d_torch[1]);}
		if(dt2){k = dimmer(d_torch[2]);}
		if(dt3){k = dimmer(d_torch[3]);}
		if(dt4){k = dimmer(d_torch[4]);}
		if(dt0){k = dimmer(d_torch[0]);}


		if(p > k){
        color= in_color*p;
		}else{
		//if (in_color[0] <= in_color[0]*k || in_color[1] <= in_color[1]*k || in_color[2] <= in_color[2]*k || in_color[3] <= in_color[3]*k){
		color= in_color*k;
		}

      }
      else if (x <= x0+w && x >= x0-w && y <= y0+h && y >= y0-h && text_cond == 1){
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
      vec4 i= vec4(1,0,0,0.7);
      if (d <= 100*100){
        color= i*in_color;
      }
      else if (x <= x2+wr && x >= x2-wr && y <= y2+h && y >= y2-h && re_cond == 0){
      color= in_color;
      }else {
        color= i;
      }

    }
//    color= in_color;

}
