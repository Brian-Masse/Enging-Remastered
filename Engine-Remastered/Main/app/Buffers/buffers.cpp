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

#include "../app.h"

using namespace std;
using namespace glm;


void HelloTriangleApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    //need to temporarily allocate a command buffer for transferring vertex data between buffers
    // this allocation, to be be sped up, should be done with a commandBuffer
    VkCommandBuffer commandBuffer;
    commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void HelloTriangleApplication::createBuffer( VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory ) {
    //buffers are space in memory for data to be stored for the GPU to read
    // Memory must be managed explicitly
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = flags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //will only be used by the graphics queue, so it does not need to be shared among other queue families

    VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
    if (result != VK_SUCCESS) { throw runtime_error( "Failed to create the Buffer" ); }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize =  memRequirements.size;
    allocateInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memFlags);

    VkResult memResult = vkAllocateMemory(device, &allocateInfo, nullptr, &bufferMemory);
    if (memResult != VK_SUCCESS) { throw runtime_error( "Failed to Allocate Memory" ); }
    
    vkBindBufferMemory(device, buffer, bufferMemory, 0); 
}

void HelloTriangleApplication::mapBuffer(VkDeviceSize size, VkDeviceMemory& bufferMemory, const void* src) {

    void* data;
    vkMapMemory(device, bufferMemory, 0, size, 0, &data);
    memcpy(data, src, (size_t) size);
    vkUnmapMemory(device, bufferMemory);
}

uint32_t HelloTriangleApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { return i; }
    }

    throw runtime_error("Failed to find suitable Memory Type!");

}