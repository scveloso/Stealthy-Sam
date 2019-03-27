#version 330
layout(location = 0) in vec4 in_position;
uniform vec2 enemy_position[5];

out vec2 uv;
out vec2 en_position[5];
float en_alpha;
float en_beta;

void main()
{
    gl_Position = in_position;
    // Convert to the [0, 1] range of UV coordinate
    uv = (in_position.xy + vec2(1.05, 1.05)) / 2.1;


    for(int i = 0; i < 5; i++)
  	{
      //tuning adjustment variable based on enemy location range
      if (enemy_position[i].x <= 300){
        en_alpha = -0.95;
      } else if( 300 < enemy_position[i].x && enemy_position[i].x <= 600){
        en_alpha = -0.31;
      } else if( 600 < enemy_position[i].x && enemy_position[i].x <= 900){
        en_alpha = 0.32;
      }else if (900 < enemy_position[i].x){
        en_alpha = 0.95;
      }
     if (enemy_position[i].y <= 200){
        en_beta = 0.95;
      } else if( 200 < enemy_position[i].y && enemy_position[i].y <= 400){
        en_beta = 0.31;
      } else if( 400 < enemy_position[i].y && enemy_position[i].y <= 600){
        en_beta = -0.32;
      }else if (600 < enemy_position[i].y){
        en_beta = -0.95;
      }
      en_position[i]= vec2(enemy_position[i].x+(en_alpha*20), 800-enemy_position[i].y+(en_beta*12));
      // en_position[i]= vec2(enemy_position[i].x, 800-enemy_position[i].y);
    }


}
