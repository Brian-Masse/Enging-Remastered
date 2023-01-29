#ifndef VERTEX_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <array>

using namespace std;
using namespace glm;

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0; //index of the binding in an array of bindings (there is only one now)
        bindingDescription.stride = sizeof(Vertex);  //
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;   //which binding the per vertex data comes
        attributeDescriptions[0].location = 0;  //the location directive in the vertex shader
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT; //the amount of color channels in this macro should match the number of components for the data type (pos)
        // the type (SFLOAT, UINT, SINT) should also match data type from vertex information
        attributeDescriptions[0].offset = offsetof(Vertex, pos); // the space from one vertex to the next
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

#endif