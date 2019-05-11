#version 440

attribute highp vec4 vertex;
attribute mediump vec4 tex_coord;

varying mediump vec4 texc;
uniform mediump mat4 matrix;

void main(void)
{
    gl_Position = matrix * vertex;
    texc = tex_coord;
}
