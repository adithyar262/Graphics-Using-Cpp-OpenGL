#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

uniform vec4 u_Color;       // When drawing the cube instance, we'll set our uniform color to represent different block types.
uniform float u_Time;
uniform vec3 sunDir;

in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Nor;             // The array of vertex normals passed to the shader

in vec4 vs_Col;             // The array of vertex colors passed to the shader.

out vec4 fs_Pos;
out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_LightVec;       // The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.
out float fragDepth;        // used for fog generation

// const vec4 lightDir = normalize(vec4(0.5, 1, 0.75, 0));  // The direction of our virtual light, which is used to compute the shading of
//                                         // the geometry in the fragment shader.

float fade(float t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }
float grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
float noise(vec3 p) {
    vec3 P = floor(p);
    vec3 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    float n = P.x + P.y * 157.0 + P.z * 113.0;
    return mix(mix(mix(grad(int(n + 0.0), f.x, f.y, f.z), grad(int(n + 1.0), f.x - 1.0, f.y, f.z), f.x),
                   mix(grad(int(n + 157.0), f.x, f.y - 1.0, f.z), grad(int(n + 158.0), f.x - 1.0, f.y - 1.0, f.z), f.x), f.y),
               mix(mix(grad(int(n + 113.0), f.x, f.y, f.z - 1.0), grad(int(n + 114.0), f.x - 1.0, f.y, f.z - 1.0), f.x),
                   mix(grad(int(n + 270.0), f.x, f.y - 1.0, f.z - 1.0), grad(int(n + 271.0), f.x - 1.0, f.y - 1.0, f.z - 1.0), f.x), f.y), f.z);
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec4 vertPos = vs_Pos;
    bool bird = false;
    if(vertPos.w == 84.f) { // bird
        vertPos.w = 1.f;
        vertPos.y += sin(u_Time)/40;
        bird = true;
    }

    if(vs_Col.z == 2) {// Lava
        // float distortion = 0.1 * noise(vs_Pos.xyz * 0.5 + u_Time);
        // vertPos.y = vs_Pos.y + fs_Nor.y * distortion;
        // if(/*sin(rand(vertPos.xz)) < 0.99999 && */sin(rand(vertPos.xz)) > 0.7 && int(u_Time) % 76766 == 2) {
        //     // if(int(vertPos.x) % 15 == 0 && int(vertPos.z) % 64 == 0)
        //         vertPos.y += (int(u_Time) % 7) / 9;
        // }
        vec4 noise = vec4(sin(vertPos.x * 15.0 + u_Time), cos(vertPos.z * 15.0 + u_Time), 0.0, 1.f);
        vertPos = vertPos + noise * 0.04;
        float bubbleStrength = 0.11; // Adjust strength of bubble effect
        float bubbleFrequency = 10.0; // Adjust frequency of bubbles
        float bubbleOffset = 0.5; // Adjust offset to control bubble appearance
        vec3 bubble = vec3(sin(u_Time * bubbleFrequency + vertPos.x) * cos(u_Time * bubbleFrequency + vertPos.z),
                           cos(u_Time * bubbleFrequency + vertPos.z) * sin(u_Time * bubbleFrequency + vertPos.x),
                           0.0) * bubbleStrength * (1.0 - smoothstep(bubbleOffset - 0.05, bubbleOffset + 0.05, noise.z));

        float jumpFrequency = 0.5; // Adjust frequency of high jumping bubbles
        float jumpAmplitude = 0.1; // Adjust amplitude of high jumping bubbles
        float jumpOffset = 0.7; // Adjust offset to control appearance of high jumping bubbles
        bubble += vec3(0.0, sin(u_Time * jumpFrequency + vertPos.x) * cos(u_Time * jumpFrequency + vertPos.z),
                       sin(u_Time * jumpFrequency + vertPos.z) * cos(u_Time * jumpFrequency + vertPos.x)) *
                  jumpAmplitude * step(jumpOffset, noise.z);

        vertPos += normalize(vertPos) * vec4(bubble, 0.f);
        vertPos.w = 1.f;
    }

    if(vs_Col.z == 1) {// Water
        // pos.y += sin(pos.x * 5.0 + time) * 0.1;
        vertPos.y += sin(vertPos.x * 5.0 + u_Time) * 0.1;
    }
    // if(bird) { // bird
    //     vertPos.w = 84.f;
    //     bird = false;
    // }

    fs_Pos = vertPos;
    fs_Col = vs_Col;                         // Pass the vertex colors to the fragment shader for interpolation



    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);          // Pass the vertex normals to the fragment shader for interpolation.
                                                            // Transform the geometry's normals by the inverse transpose of the
                                                            // model matrix. This is necessary to ensure the normals remain
                                                            // perpendicular to the surface after the surface is transformed by
                                                            // the model matrix.


    vec4 modelposition = u_Model * vertPos;   // Temporarily store the transformed vertex positions for use below
    // float lightAngle = u_Time * 0.2;  // Control speed of day-night cycle
    // vec4 lightDir = vec4(0.5, cos(lightAngle), sin(lightAngle), 0.0);  // Rotate light in y-z plane
    vec4 lightDir = vec4(sunDir.x, sunDir.y, sunDir.z, 0.0); // Light opposite to sun

    fs_LightVec = normalize(lightDir);

    // fs_LightVec = (lightDir);  // Compute the direction in which the light source lies

    gl_Position = u_ViewProj * modelposition;// gl_Position is a built-in variable of OpenGL which is
                                             // used to render the final positions of the geometry's vertices

    fragDepth = gl_Position.z;
}
