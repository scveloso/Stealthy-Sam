#version 330
layout(location = 0) in vec4 in_position;
//uniform vec2 sam_position;
uniform vec2 e_position;
uniform vec2 text_position;
uniform vec2 r_position;
uniform vec2 circle_light_position;
uniform vec2 enemy_position[5];
uniform vec2 torch_light[5];



out vec2 uv; out vec2 s_position;
out vec2 keye_position; out vec2 re_position;
out vec2 t_position;
out vec2 en_position[5];
out vec2 tor_position[5];

//adjustment variable
float alpha;
float beta;
float en_alpha;
float en_beta;
float tor_alpha;
float tor_beta;
void main()
{
    gl_Position = in_position;
    // Convert to the [0, 1] range of UV coordinate
    uv = (in_position.xy + vec2(1.05, 1.05)) / 2.1;

    // tuning adjustment variable based on sam location range
    if (circle_light_position.x <= 300){
      alpha = -0.95;
    } else if( 300 < circle_light_position.x && circle_light_position.x <= 600){
      alpha = -0.31;
    } else if( 600 < circle_light_position.x && circle_light_position.x <= 900){
      alpha = 0.32;
    }else if (900 < circle_light_position.x){
      alpha = 0.95;
    }
   if (circle_light_position.y <= 200){
      beta = 0.95;
    } else if( 200 < circle_light_position.y && circle_light_position.y <= 400){
      beta = 0.31;
    } else if( 400 < circle_light_position.y && circle_light_position.y <= 600){
      beta = -0.32;
    }else if (600 < circle_light_position.y){
      beta = -0.95;
    }

    s_position= vec2(circle_light_position.x+(alpha*20), 800-circle_light_position.y+(beta*12));
    keye_position= vec2(e_position.x-15, 800-e_position.y+12);
    t_position= vec2(text_position.x-15, 800-text_position.y+12);
    re_position= vec2(r_position.x-5, 800-r_position.y+12);

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

    for(int i = 0; i < 5; i++)
  	{
      //tuning adjustment variable based on torch location range
      if (torch_light[i].x <= 300){
        tor_alpha = -0.95;
      } else if( 300 < torch_light[i].x && torch_light[i].x <= 600){
        tor_alpha = -0.31;
      } else if( 600 < torch_light[i].x && torch_light[i].x <= 900){
        tor_alpha = 0.32;
      }else if (900 < torch_light[i].x){
        tor_alpha = 0.95;
      }
     if (torch_light[i].y <= 200){
        tor_beta = 0.95;
      } else if( 200 < torch_light[i].y && torch_light[i].y <= 400){
        tor_beta = 0.31;
      } else if( 400 < torch_light[i].y && torch_light[i].y <= 600){
        tor_beta = -0.32;
      }else if (600 < torch_light[i].y){
        tor_beta = -0.95;
      }
      tor_position[i]= vec2(torch_light[i].x+(tor_alpha*20), 800-torch_light[i].y+(tor_beta*12));
    }

}
