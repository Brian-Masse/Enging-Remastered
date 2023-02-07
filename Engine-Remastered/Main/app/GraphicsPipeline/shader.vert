#version 450

//location of the frame buffer
// These 2 inputs are vertex specific atributes
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

vec3 cameraPos = vec3(0, 0, -1);
float n = 0.7;

float test = 0;

//gets called for every vertex  
void main() {

    float x = inPosition.x;
    float y = inPosition.y;
    float z = inPosition.z + 1;

    float yf = (n * y) / z;
    float xf = (n * x) / z;
    

    // gl_Position = vec4(xf, yf, z, 1.0);
    gl_Position = vec4(inPosition.x, inPosition.y, 0.5, 1.0);
    fragColor = inColor;
}
