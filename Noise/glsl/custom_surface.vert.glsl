#version 330 core

uniform mat4 u_View;
uniform mat4 u_Proj;

uniform float u_Time;

in vec3 vs_Pos;
in vec3 vs_Nor;
in vec2 vs_UV;

out vec3 fs_Pos;
out vec3 fs_Nor;
out vec2 fs_UV;

// Perlin noise function
float noise(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()
{
    // Deform vertex position using Perlin noise
    float deformationAmount = 0.1; // Adjust this value for the intensity of deformation
    float noiseScale = 0.5; // Adjust this value for the scale of noise
    vec3 noiseOffset = vec3(0.0, 0.0, 0.0); // Adjust this value for additional variation
    vec3 deformation = vec3(
        noise(noiseScale * (vs_Pos + noiseOffset + vec3(u_Time * 0.1, 0.0, 0.0))),
        noise(noiseScale * (vs_Pos + noiseOffset + vec3(0.0, u_Time * 0.1, 0.0))),
        noise(noiseScale * (vs_Pos + noiseOffset + vec3(0.0, 0.0, u_Time * 0.1)))
    );
    vec3 deformedPosition = vs_Pos + deformationAmount * deformation;

    // Pass variables to fragment shader
    fs_Pos = deformedPosition;
    fs_Nor = vs_Nor;
    fs_UV = vs_UV;

    // Apply projection and view matrices
    gl_Position = u_Proj * u_View * vec4(deformedPosition, 1.0);
}
