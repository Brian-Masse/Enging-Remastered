// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>

// std 
#include <algorithm>
#include <vector>
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../app/Buffers/vertexBuffer.h"

double theta = 30;

vector<Vertex>& translateVertices(vector<Vertex>& vertices) {

    vec3 cameraPos = { 0, 0, -1 };

    for ( auto& vertex : vertices ) {

        double x = vertex.pos.x - cameraPos.x ;
        double y = vertex.pos.y - cameraPos.y;
        double z = vertex.pos.z - cameraPos.z;

        double n = 0.7;

        double yf = (n * y) / z;
        double xf = (n * x) / z;

        vertex.pos.x = xf;
        vertex.pos.y = yf;
        vertex.pos.z = z;


        
    //     double x = vertex.pos.x;
    //     double z = vertex.pos.z;
        
    //     double offsetX = z * t;
    //     int dir = -1 * (x / abs( x ));

    //     double newX = (offsetX * dir) + x;
    //     vertex.pos.x = newX;


    //     double y = vertex.pos.y;
        
    //     double offsetY = z * t;
    //     int dirY = -1 * (y / abs( y ));

    //     double newY = (offsetY * dirY) + y;
    //     vertex.pos.y = newY;
    }

    return vertices;

}