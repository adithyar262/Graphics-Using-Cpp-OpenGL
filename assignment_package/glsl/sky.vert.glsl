#version 150

in vec4 vs_Pos;
uniform float u_Time;
uniform vec3 sunDir;
out float yCoord;


void main()
{
    gl_Position = vs_Pos;
    yCoord = gl_Position.y;
}
