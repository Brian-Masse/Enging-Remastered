#version 450

//location of the frame buffer
// These 2 inputs are vertex specific atributes
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inColor;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec2 inUV;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;

layout(binding = 0) uniform UniformConstantData {
    vec3 cameraPos;
} push;

float n = 0.7;

vec3 light = vec3( -1, -0.7, 0.5 );

//gets called for every vertex  
void main() {

    float nx = -inNormal.x;
    float ny = -inNormal.y;
    float nz = -inNormal.z;

    float vertexNormalMagnitude = sqrt( pow(nx, 2) + pow(ny, 2) + pow(nz, 2) );
    float LightNormalMagnitude = sqrt( pow(light.x, 2) + pow(light.y, 2) + pow(light.z, 2) );

    float num = (light.x * nx) + (light.y * ny) + (light.z * nz);
    float proj = num / LightNormalMagnitude;
    float perc = proj / vertexNormalMagnitude;


    float x = (inPosition.x / 1.8) - push.cameraPos.x;
    float y = (inPosition.y / 1.8) - push.cameraPos.y;
    float z = (inPosition.z / 1.8) - push.cameraPos.z;

    float yf = (n * y) / z;
    float xf = (n * x) / z;
    

    gl_Position = vec4(xf, yf, z / 2, 1.0);
    fragColor = vec3( inColor.r * perc, inColor.g * perc, inColor.b * perc );
    fragUV = inUV;
    // fragColor = vec3(0.1, 0.1, 0.1);
    // fragColor = inColor;
}
