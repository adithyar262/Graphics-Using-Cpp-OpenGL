#version 330 core

uniform mat4 u_MVP; // Model-View-Projection matrix

in vec3 vs_Pos; // Vertex position
in vec3 vs_Nor; // Vertex normal
in vec2 vs_UV; // Vertex texture coordinate

out vec3 fs_Pos; // World-space position data passed down to fragment shader
out vec3 fs_Nor; // Surface normal passed down to the fragment shader
out vec2 fs_UV; // Texture coordinate passed down to the fragment shader
out vec3 fs_ViewDir; // View direction passed down to fragment shader
out vec3 fs_LightDir; // Light direction passed down to fragment shader

void main()
{
    fs_Pos = vs_Pos;
    fs_UV = vs_UV;
    fs_Nor = normalize(vs_Nor);

    // Calculate view direction
    fs_ViewDir = normalize(-fs_Pos);

    // Assuming a point light source at position (0, 0, 0)
    fs_LightDir = normalize(vec3(0.0) - fs_Pos);

    gl_Position = u_MVP * vec4(vs_Pos, 1.);
}
