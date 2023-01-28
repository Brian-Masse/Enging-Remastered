#version 450


//temporary hardcoding vertex positions
vec2 positions[4] = vec2[](
    vec2(-0.5, -0.5),
    vec2(0.5, -0.5),
    vec2(-0.5, 0.5),
    vec2(0.5, 0.5)
);

vec3 colors[4] = vec3[](
    vec3(1.0, 0.23921569, 0.23921569),
    vec3(0.37254902, 0.6, 0.96078431),
    vec3(0.85882353, 0.89019608, 0.62352941),
    vec3(0.0, 1.0, 1.0)
);

//location of the frame buffer
layout(location = 0) out vec3 fragColor;

//gets called for every vertex  
void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[ gl_VertexIndex ];
}



// vec2 positions[4] = vec2[](
//     vec2(-0.5, 0.5),
//     vec2(-0.5, -0.5),
//     vec2(0.5, -0.5),

//     vec2(0.5, -0.5),
//     vec2(0.5, 0.5),
//     vec2(-0.5, 0.5)
// );

// vec3 colors[4] = vec3[](
//     vec3(1.0, 0.23921569, 0.23921569),
//     vec3(0.37254902, 0.6, 0.96078431),
//     vec3(0.85882353, 0.89019608, 0.62352941),

//     vec3(0.85882353, 0.89019608, 0.62352941),
//     vec3(1.0, 1.0, 1.0),
//     vec3(1.0, 0.23921569, 0.23921569)
// );