#version 330 core

uniform ivec2 u_ScreenDimensions;
uniform float u_Time;
uniform ivec2 u_MousePosition;
out vec4 out_Col;

void main() {
    float width = float(u_ScreenDimensions.x);
    float height = float(u_ScreenDimensions.y);
    vec2 center = vec2(width / 2.0, height / 2.0);
    vec2 position = gl_FragCoord.xy;
    float radius = min(width, height) / 2.0;
    float distanceFromCenter = length(position - center);
    float timeColor = cos(u_Time) * 0.5 + 0.5; // color also dependent on time
    // out_Col = vec4(1.0, 0.0, 0.0, 1.0);
    float blendFactor = distanceFromCenter / radius;
    vec3 blueColor = vec3(position.y / height, position.x / width, 1 - timeColor); // black
    vec3 outsideColor = vec3(position.x / width, position.y / height, timeColor);
    vec3 blendedColor = mix(blueColor, outsideColor, blendFactor);
    out_Col = vec4(blendedColor, 1.0);
}
