#version 330 core

uniform mat4 u_View;  // View matrix
uniform mat4 u_Proj;  // Projection matrix

in vec3 vs_Pos;       // Vertex position
in vec3 vs_Nor;       // Vertex normal

out vec2 fs_UV;       // UV coordinates for fragment shader

void main()
{
    // Transform vertex position to clip space
    gl_Position = u_Proj * u_View * vec4(vs_Pos, 1.0);

    // Transform vertex normal to camera space
    vec3 normalCamSpace = mat3(u_View) * vs_Nor;

    // Convert the normal to UV coordinates
    float u = 0.5 + atan(normalCamSpace.z, normalCamSpace.x) / (2.0 * 3.14159);
    float v = 0.5 - asin(normalCamSpace.y) / 3.14159;

    // Output UV coordinates
    fs_UV = vec2(u, v);
}
