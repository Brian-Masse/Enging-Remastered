// runs on every fragment genreated by the vertext shader
//species color, depth, to the frame buffer

#version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec3 fragColor; 

void main() {
    outColor = vec4(fragColor, 1.0);
}