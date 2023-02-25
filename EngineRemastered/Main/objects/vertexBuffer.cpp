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

#include "object.h"
#include "universalConstructors.h"

using namespace std;
using namespace glm;


void EngineObject::createVertexBuffer() {
    VkDeviceSize size = sizeof( vertices[0] ) * vertices.size();

    VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    //int he future, individual memory allocation for every object is bad, and can reach physical device limitations
    //instead, create an allocator, that allocates space for 'one big object', then use the offset perimeters in functions to subdivide this allocation 
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;
    createBuffer(info, size, stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingMemory);
    mapBuffer(info, size, stagingMemory, vertices.data());

    createBuffer(info, size, usageFlags, memoryFlags, vertexBuffer, vertexBufferMemory);
    copyBuffer(info, stagingBuffer, vertexBuffer, size);  //copy the vertex data from CPU-accessible memory to optimized GPU only memory

    vkDestroyBuffer(info.device, stagingBuffer, nullptr);
    vkFreeMemory(info.device, stagingMemory, nullptr);
}

void EngineObject::createIndexBuffer() {
    VkDeviceSize size = sizeof( indices[0] ) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(info, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    mapBuffer(info, size, stagingBufferMemory, indices.data());
    
    createBuffer(info, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
    copyBuffer(info, stagingBuffer, indexBuffer, size);

    vkDestroyBuffer(info.device, stagingBuffer, nullptr);
    vkFreeMemory(info.device, stagingBufferMemory, nullptr);

}