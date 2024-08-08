#version 150

in vec4 vs_Pos;
uniform float blockType;


void main() {
    gl_Position = vs_Pos + vec4(-1.85, -1.7, 0.0, 0.0);
}
