#version 330
layout(location = 0) in vec4 in_position;
uniform vec2 sam_position;
// uniform vec2 text_position;
// uniform vec2 e_position;

out vec2 uv; out vec2 s_position; //out float d;
// out vec2 ep; out vec2 tp;

void main()
{
    gl_Position = in_position;
    //float d= (gl_Position.x - sam_position.x)*(gl_Position.x - sam_position.x)+(gl_Position.y- sam_position.y)*(gl_Position.y-sam_position.y);

    // Convert to the [0, 1] range of UV coordinate
    uv = (in_position.xy + vec2(1.05, 1.05)) / 2.1;
    //s_position= vec2(sam_position.x, sam_position.y);
    s_position= vec2(sam_position.x+20, 800-sam_position.y);

}
