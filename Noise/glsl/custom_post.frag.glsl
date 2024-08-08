#version 330 core

uniform ivec2 u_Dimensions;
uniform float u_Time;

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_Texture;

// Function to generate 2D Worley noise
float worleyNoise(in vec2 uv) {
    vec2 p = floor(uv);
    vec2 f = fract(uv);

    // Distance to the closest cell center
    vec3 d = vec3(1.0);
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            vec2 neighbor = vec2(float(i), float(j));
            vec2 randomOffset = vec2(
                0.5 - cos(0.5 * u_Time),
                0.5 - sin(0.5 * u_Time)
            );
            vec2 pOffset = p + neighbor + randomOffset;
            vec3 neighborPoint = vec3(pOffset, 0.0);
            vec2 offset = fract(neighborPoint.xy) - f;
            float d2 = dot(offset, offset);
            d = min(d, vec3(d2, neighborPoint));
        }
    }
    return sqrt(d.x);
}

// Function to generate glitch effect
vec2 glitchUV(vec2 uv, float time) {
    float dist = length(uv - vec2(0.5));
    uv += 0.02 * sin(time * 10.0 + dist * 20.0);
    return uv;
}

float noise(in vec2 p) {
    return fract(sin(dot(p, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

    // Apply Worley noise to UV coordinates
    vec2 worleyUV = fs_UV + 0.05 * worleyNoise(fs_UV * u_Dimensions.xy);

    // Apply glitch effect to UV coordinates
    vec2 distortedUV = glitchUV(worleyUV, u_Time);

    // Sample color from original image at distorted UV coordinates
    vec3 distortedColor = texture(u_Texture, distortedUV).rgb;

    // Apply radial blur
    vec2 center = vec2(0.5);
    float blurAmount = 1.0 - length(fs_UV - center);
    float blurFactor = 0.02; // Adjust blur strength
    for (int i = 0; i < 10; i++) {
        vec2 offset = vec2(cos(u_Time * 2.0 + float(i)), sin(u_Time * 10.0 + float(i))) * blurAmount * blurFactor;
        distortedColor += texture(u_Texture, distortedUV + offset).rgb;
    }
    distortedColor /= 10.0; // Average color samples

    // Output the final color
    color = distortedColor;

}
