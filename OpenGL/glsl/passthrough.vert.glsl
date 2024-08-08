#version 330 core

// The position of the vertex being
// processed by this instance of the vertex shader
in vec3 vs_Pos;
// The width and height of your canvas in pixels
uniform ivec2 u_ScreenDimensions;
// A homogeneous 2D transformation matrix
uniform mat3 u_Model;
// A constantly-increasing value updated in MyGL::tick()
uniform float u_Time;

void main()
{
    // TODO: Add code that scales your vertex's X coordinate
    // based on your screen's aspect ratio to correct for the
    // fact that screen space is uniformly [-1,1] in X and Y
    // regardless of aspect ratio.

    // Example code: Passes the input vertex position down
    // the pipeline with a W coordinate of 1.
    // aspect ratio
    float aspectRatio = float(u_ScreenDimensions.x) / float(u_ScreenDimensions.y);
    vec3 adjustedPos = vs_Pos;
    // divide x by aspect ratio
    adjustedPos.x = adjustedPos.x / aspectRatio;
    vec3 transformedPos = u_Model * vec3(adjustedPos.x, adjustedPos.y, 1.0);

    gl_Position = vec4(transformedPos , 1.0);
}
