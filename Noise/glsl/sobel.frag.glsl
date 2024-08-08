#version 330 core

in vec2 fs_UV;
out vec3 color;

uniform sampler2D u_Texture;


void main() {
    // TODO: Use texelFetch to sample u_Texture
    // at each of the nine pixels at and surrounding
    // this shader's fragment. Multiply each sample by the
    // associated cell in Gx, and add it to a sum of horizontal
    // color deltas. Do the same for Gy and a sum of vertical
    // color deltas. Then, output the square root of the sum
    // of both deltas squared.

    // Sobel kernels
    mat3 Gx = mat3(3, 0, -3,
                   10, 0, -10,
                   3, 0, -3);

    mat3 Gy = mat3(3, 10, 3,
                   0, 0, 0,
                   -3, -10, -3);

    vec3 colorSumX = vec3(0.f);
    vec3 colorSumY = vec3(0.f);

    // Apply Sobel filter
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            vec3 sample = texelFetch(u_Texture, ivec2(gl_FragCoord.xy) + ivec2(i, j), 0).rgb;
            colorSumX += sample * Gx[i + 1][j + 1];
            colorSumY += sample * Gy[i + 1][j + 1];
        }
    }

    // Compute magnitude of gradient
    float magnitude = sqrt(dot(colorSumX, colorSumX) + dot(colorSumY, colorSumY));

    // Output the magnitude
    color = texture(u_Texture, fs_UV).rgb * vec3(magnitude);

    // color = texelFetch(u_Texture, ivec2(gl_FragCoord.xy), 0).rgb;
}
