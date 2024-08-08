#version 330 core

uniform float u_Time;

in vec3 vs_Pos;
in vec2 vs_UV;
out vec2 fs_UV;

void main()
{
    fs_UV = vs_UV;
    gl_Position = vec4(vs_Pos + vec3(cos(u_Time * 0.05), 0.0, cos(u_Time * 0.03)/*, 0.0, 0.0*/), 1.0);
}
