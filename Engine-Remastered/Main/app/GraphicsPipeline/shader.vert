#version 450

//location of the frame buffer
// These 2 inputs are vertex specific atributes
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

//gets called for every vertex  
void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor;
}
