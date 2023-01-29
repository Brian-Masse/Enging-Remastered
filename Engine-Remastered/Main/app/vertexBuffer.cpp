#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <array>

#include "app.h"

using namespace std;
using namespace glm;

void HelloTriangleApplication::createVertexBuffer() {

    //buffers are space in memory for data to be stored for the GPU to read
    // Memory must be managed explicitly
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof( vertices[0] ) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //will only be used by the graphics queue, so it does not need to be shared among other queue families

    VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer);
    if (result != VK_SUCCESS) { throw runtime_error( "Failed to create the VertexBuffer" ); }

    VkMemoryRequirements vertexMemRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &vertexMemRequirements);

    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize =  vertexMemRequirements.size;
    allocateInfo.memoryTypeIndex = findMemoryType(vertexMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkResult memResult = vkAllocateMemory(device, &allocateInfo, nullptr, &vertexBufferMemory);
    if (memResult != VK_SUCCESS) { throw runtime_error( "Failed to Allocate Memory" ); }
    
    vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0); 
    mapVertices();
}

void HelloTriangleApplication::mapVertices() {

    VkDeviceSize size = sizeof( vertices[0] ) * vertices.size();

    void* data;
    vkMapMemory(device, vertexBufferMemory, 0, size, 0, &data);
    memcpy(data, vertices.data(), (size_t) size);
    vkUnmapMemory(device, vertexBufferMemory);
}

uint32_t HelloTriangleApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { return i; }
    }

    throw runtime_error("Failed to find suitable Memory Type!");

}

