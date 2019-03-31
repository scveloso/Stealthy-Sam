#version 330
layout(location = 0) in vec4 in_position;
uniform vec2 circle_light_position;
uniform vec2 torch_light[5];

out vec2 uv;
out vec2 s_position;
out vec2 tor_position[5];

float alpha;
float beta;
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
