#version 330 core

uniform ivec2 u_Dimensions;
uniform float u_Time;

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_Texture;

float noise(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main() {
    // Get the distorted UV coordinates using Perlin noise
    vec2 a = fs_UV * 10.0 + vec2(u_Time, u_Time);
    vec2 c = fs_UV * 10.0 + vec2(u_Time * 0.5, u_Time * 0.7);
    float noise1 = 0.01 * noise(vec3(a, 1.0f));
    float noise2 = 0.01 * noise(vec3(c, 1.0f));
    vec2 distortedUV = fs_UV + vec2(noise1, noise2);
    float r = texture(u_Texture, distortedUV + vec2(0.002, 0.0)).r;
    float g = texture(u_Texture, distortedUV).g;
    float b = texture(u_Texture, distortedUV + vec2(-0.002, 0.0)).b;
    vec3 distortedColor = vec3(r, g, b);

    // Apply CRT-TV effect using a sine curve
    float crtEffect = 0.5 + 0.5 * sin(50.0 * fs_UV.y + u_Time); // Adjust the frequency with the value 50.0
    distortedColor *= crtEffect;

    // Output the final color
    // color = texture(u_Texture, fs_UV).rgb;
    color = distortedColor;
}
