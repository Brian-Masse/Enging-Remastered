#ifndef OBJECT_H
#define OBJECT_H

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
#include <optional>
#include <set>

using namespace std;

class EngineObject {

    public:


    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec2 UV;
        

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0; //index of the binding in an array of bindings (there is only one now)
            bindingDescription.stride = sizeof(Vertex);  //
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;



            return bindingDescription;
        }

        static array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
            array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};

            setDescription( attributeDescriptions, 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos));
            setDescription( attributeDescriptions, 0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));
            setDescription( attributeDescriptions, 0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal));
            setDescription( attributeDescriptions, 0, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV));

            return attributeDescriptions;
        }

        static void setDescription( array<VkVertexInputAttributeDescription, 4>& attributes, int binding, int location, VkFormat format, uint32_t offset ) {
            attributes[location].binding = binding;
            attributes[location].location = location;
            attributes[location].format = format;
            attributes[location].offset = offset;
        }
    };

    struct BufferInformation {
        // int vertexSize;
        // int vertexSize; //how many vertices in each face

        // int indexSize;
        std::vector<Vertex> vertexBuffer;
        std::vector<uint16_t> indexBuffer;
    };

    struct faces {
        vector< Vertex > vertices;
    };


    EngineObject() { }

    private:

};

#endif