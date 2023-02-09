// runs on every fragment genreated by the vertext shader
//species color, depth, to the frame buffer

#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor; 
layout(location = 1) in vec2 UV; 

layout(binding = 1) uniform sampler2D textureSampler;

void main() {

    

    // outColor = vec4(UV, 0.0, 1.0);
    outColor = vec4(fragColor * texture(textureSampler, vec2( -UV.y, -UV.x )).rgb, 1.0);
}