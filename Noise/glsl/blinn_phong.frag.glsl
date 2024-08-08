#version 330 core

uniform sampler2D u_Texture; // Texture sampler
uniform vec3 u_CamLook;      // Camera's forward vector
uniform vec3 u_CamPos;       // Camera's position

in vec3 fs_Pos;  // Fragment position
in vec3 fs_Nor;  // Fragment normal
in vec2 fs_UV;   // Fragment texture coordinates

out vec4 out_Color; // Output color

float ComputeSpecularIntensity(vec3 viewDir, vec3 lightDir, vec3 normal, float exp) {
    vec3 H = normalize(lightDir + viewDir);
    return max(pow(dot(H, normal), exp), 0.0);
}

void main()
{
    // Retrieve texture color
    vec4 albedo = texture(u_Texture, fs_UV);

    vec3 direction = normalize(u_CamPos - u_CamLook);

    // Blinn-Phong reflection
    float specularIntensity = ComputeSpecularIntensity(u_CamPos, u_CamLook, fs_Nor, 10);

    // Final color calculation
    vec3 ambient = albedo.rgb * 0.2;
    vec3 diffuse = albedo.rgb * max(dot(fs_Nor, -u_CamLook), 0.0);
    vec3 specular = /*vec3(1.0, 1.0, 1.0)*/albedo.rgb * specularIntensity;

    vec3 finalColor = ambient + diffuse + specular;

    out_Color = vec4(finalColor, albedo.a);
}
