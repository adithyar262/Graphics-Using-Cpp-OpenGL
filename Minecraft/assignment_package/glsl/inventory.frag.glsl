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
out vec4 outCol;
uniform float blockType;

void main()
{
    if (blockType == 0) {
        // GRASS
        outCol = vec4(95.f, 159.f, 53.f,  255.f)/ 255.f;
    } else if (blockType == 1) {
        // DIRT
        outCol = vec4(121.f, 85.f, 58.f,  255.f) / 255.f;
    } else if (blockType == 2) {
        // STONE
        outCol = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    } else if (blockType == 3) {
        // WATER
        outCol = vec4(0.f, 0.f, 0.75f, 1.0f);

    } else if (blockType == 4) {
        // SNOW
        outCol = vec4(240.f, 240.f, 240.f,  255.f)/ 255.f;

    } else if (blockType == 5) {
        // SAND
        outCol = vec4(194.f, 178.f, 128.f,  255.f)/ 255.f;

    } else if (blockType == 6) {
        // LAVA
        outCol = vec4(178.f, 34.f, 34.f,  255.f)/ 255.f;

    } else if (blockType == 7) {
        // BEDROCK
        outCol = vec4(20.f, 20.f, 20.f, 255.f)/ 255.f;
    } else {
        // dubugging pink  RGB(255, 182, 193)
        outCol = vec4(255.f, 182.f, 193.f, 255.f) / 255.f;
    }


}
