#version 330

uniform sampler2D u_Texture; // The texture to be read from by this shader
uniform float u_Time;

in vec3 fs_Pos;
in vec3 fs_Nor;
in vec2 fs_UV;

layout(location = 0) out vec3 out_Color;

void main()
{
    // Sample the texture
    vec3 texColor = texture(u_Texture, fs_UV).rgb;

    // Apply pulsating effect based on time
    float pulsate = abs(sin(u_Time)) * 0.2 + 0.8; // Adjust amplitude and frequency as needed

    // Apply the pulsating effect to the texture color
    texColor *= pulsate;

    // Output the final color
    // out_Color = vec3(1.f, 1.f, 1.f);
    out_Color = vec3(texColor);
}
