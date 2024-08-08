#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please
// open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute
// the shading of geometry. For every pixel in your program's output
// screen, the fragment shader is run for every bit of geometry that
// particular pixel overlaps. By implicitly interpolating the position
// data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex
// position, light position, and vertex color.

// uniform vec4 u_Color; // The color with which to render this instance of geometry.
uniform sampler2D u_Texture;
uniform float u_Time;
uniform vec3 sunPos;

// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Pos;
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;
in float fragDepth;

out vec4 out_Col; // This is the final output color that you will see on your
// screen for the pixel that is currently being processed.

const float fogStart = 100.0f;
const float fogEnd = 400.0f;
const vec4 fogColor = vec4(0.6, 0.7, 0.8, 1.0);

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    // Material base color (before shading)

    vec2 uv = fs_Col.xy;

    vec2 offset = vec2(0, 0);

    // animated water / lava
    if (fs_Col.z != 0) {

        offset.x = int(u_Time) % 100 * 0.01 / 16;
        uv += offset;
    }
    vec4 diffuseColor = texture(u_Texture, uv);
    // if(fs_Pos.w == 84.f) {
    //     diffuseColor = fs_Col;
    //     // fs_Pos.w = 1.f;
    // } else {
    //     diffuseColor = texture(u_Texture, uv);
    // }

    vec3 finalColor = diffuseColor.rgb;

    if (fs_Col.z == 2) { // Water

        // float noise = rand(fs_Pos.xy * 0.1) * 0.1; // Adjust the factor for intensity
        // finalColor = mix(vec3(1.0, 0.0, 0.0), vec3(0.8, 0.0, 0.0), noise);
        finalColor *= 0.5 + 0.5 * sin(fs_Pos.x * 5.0 + u_Time);
    }
    if (fs_Col.z == 2) {

        // float noise = rand(fs_Pos.xy * 0.1) * 0.1; // Adjust the factor for intensity
        // finalColor = mix(vec3(1.0, 0.0, 0.0), vec3(0.8, 0.0, 0.0), noise);
    }

    //vec4 diffuseColor = vec4(fs_Col.rg, fs_Nor.b, 1);

    // out_Col = diffuseColor;
    // out_Col = vec4(diffuseColor, 1.f);
    // return;

    // vec4 diffuseColor = fs_Col;

    // Calculate the diffuse term for Lambert shading
    vec3 diffuseLight = vec3(dot(normalize(fs_Nor), normalize(fs_LightVec)));
    // Avoid negative lighting values
    diffuseLight = clamp(diffuseLight, 0, 1) * vec3(255, 255, 190) / 255.0;

    vec3 ambientLight = vec3(0.5) * vec3(144, 96, 144) / 255.0;

    // Compute final shaded color
    vec4 color = vec4(diffuseLight + ambientLight, 1) * (0.7 * diffuseColor + 0.3 * vec4(finalColor, 1.f));
    float fogFactor = clamp((fogEnd - fragDepth) / (fogEnd - fogStart), 0.0, 1.0);
    out_Col = mix(fogColor, color, fogFactor);
}
