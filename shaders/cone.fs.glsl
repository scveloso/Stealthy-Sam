#version 330
uniform sampler2D screen_texture;
uniform float time;
uniform vec2 enemy_direction[5];
uniform int death_cond;
//uniform int color_cond;
uniform vec2 color_cond[5];

in vec2 en_position[5];
in vec2 uv;
layout(location = 0) out vec4 color;


float x= gl_FragCoord.x;
float y= gl_FragCoord.y;

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
vec4 in_color = texture(screen_texture, uv.xy);

vec2 p0;
vec2 p1;
vec2 p2;

void main()
{
  int k=0;
  for(int i = 0; i < 5; i++)
  { // calculate which direction the enemy cone should be placed
    float direction= enemy_direction[i].x;
    if (direction == 1){
      p0= vec2(en_position[i].x-displace, en_position[i].y+vdisplace);
      p1= p0+ vec2(-tri_w, -tri_h);
      p2= p1 + vec2(0,tri_h*2);
    }
    else if (direction == 2){
      p0= vec2(en_position[i].x+displace, en_position[i].y+vdisplace);
      p1= p0+ vec2(tri_w, -tri_h);
      p2= p1 + vec2(0,tri_h*2);
    }
    else if (direction == 3){
      p0= vec2(en_position[i].x, en_position[i].y+vdisplace+5);
      p1= p0+ vec2(tri_h, tri_w);
      p2= p1 + vec2(-1*tri_h*2,0);
    }
    else if (direction == 4){

      p0= vec2(en_position[i].x, en_position[i].y-vdisplace-5);
      p1= p0+ vec2(tri_h, -tri_w);
      p2= p1 + vec2(-1*tri_h*2,0);
    } else{
      p0= vec2(en_position[i].x-displace, en_position[i].y+vdisplace);
      p1= p0+ vec2(-tri_w, -tri_h);
      p2= p1 + vec2(0,tri_h*2);
    }
    //calculate the 3 vertex points (p0, p1, p2) of the view cone/traingle
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
      // color= in_color;
        if (st1 && color_cond[1].x == 1){color= vec4(1.5,1.5,0,1) *in_color;}
        else if (st2 && color_cond[2].x == 1){color= vec4(1.5,1.5,0,1) *in_color;}
        else if (st3 && color_cond[3].x == 1){color= vec4(1.5,1.5,0,1) *in_color;}
        else if (st4 && color_cond[4].x == 1){color= vec4(1.5,1.5,0,1) *in_color;}
        else if (st0 && color_cond[0].x == 1){color= vec4(1.5,1.5,0,1) *in_color;}
      else{
        color= in_color;
      }

    }
    else{
      color= vec4(0,0,0,0.1)*in_color;
    }
  }
  else{
    color= vec4(0,0,0,0.1)*in_color;
  }
}
