#version 330
layout(location = 0) in vec4 in_position;
//uniform vec2 sam_position;
uniform vec2 e_position;
uniform vec2 text_position;
uniform vec2 r_position;

out vec2 uv;
out vec2 keye_position; out vec2 re_position;
out vec2 t_position;


void main()
{
    gl_Position = in_position;
    // Convert to the [0, 1] range of UV coordinate
    uv = (in_position.xy + vec2(1.05, 1.05)) / 2.1;

    keye_position= vec2(e_position.x-15, 800-e_position.y+12);
    t_position= vec2(text_position.x-15, 800-text_position.y+12);
    re_position= vec2(r_position.x-15, 800-r_position.y+12);

}
