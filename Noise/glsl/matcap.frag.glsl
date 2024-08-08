#version 330 core

uniform sampler2D u_Texture; // Matcap texture

in vec2 fs_UV;               // UV coordinates

layout(location = 0) out vec3 out_Col; // Output color

void main()
{
    // Sample the matcap texture using UV coordinates
    vec3 matcapColor = texture(u_Texture, fs_UV).rgb;

    // Output the sampled color
    out_Col = matcapColor;
}
