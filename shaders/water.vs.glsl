#version 330
layout(location = 0) in vec4 in_position;
//uniform vec2 sam_position;
uniform vec2 e_position;
uniform vec2 text_position;
uniform vec2 r_position;
uniform vec2 circle_light_position;
uniform vec2 enemy_position[5];



out vec2 uv; out vec2 s_position;
out vec2 keye_position; out vec2 re_position;
out vec2 t_position;
out vec2 en_position[5];
void main()
{
    gl_Position = in_position;
    //float d= (gl_Position.x - circle_light_position.x)*(gl_Position.x - circle_light_position.x)+(gl_Position.y- circle_light_position.y)*(gl_Position.y-circle_light_position.y);

    // Convert to the [0, 1] range of UV coordinate
    uv = (in_position.xy + vec2(1.05, 1.05)) / 2.1;
    s_position= vec2(circle_light_position.x+20, 800-circle_light_position.y);
    keye_position= vec2(e_position.x-15, 800-e_position.y+12);
    t_position= vec2(text_position.x-15, 800-text_position.y+12);
    re_position= vec2(r_position.x-5, 800-r_position.y+12);
    for(int i = 0; i < 5; i++)
  	{
      en_position[i]= vec2(enemy_position[i].x-20, 800-enemy_position[i].y);
    }

}
