#version 440

attribute highp vec4 vertex;
attribute mediump vec4 tex_coord;

varying mediump vec4 texc;

void main(void)
{
    gl_Position = vertex;
    texc = tex_coord;
}
