#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <array>

#include "../app.h"

using namespace std;
using namespace glm;

void HelloTriangleApplication::createVertexBuffer() {
    VkDeviceSize size = sizeof( vertices[0] ) * vertices.size();

    VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    //int he future, individual memory allocation for every object is bad, and can reach physical device limitations
    //instead, create an allocator, that allocates space for 'one big object', then use the offset perimeters in functions to subdivide this allocation 
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;
    createBuffer(size, stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingMemory);
    mapVertices(size, stagingMemory, vertices.data());

    createBuffer(size, usageFlags, memoryFlags, vertexBuffer, vertexBufferMemory);
    copyBuffer(stagingBuffer, vertexBuffer, size);  //copy the vertex data from CPU-accessible memory to optimized GPU only memory

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingMemory, nullptr);

}

void HelloTriangleApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    //need to temporarily allocate a command buffer for transferring vertex data between buffers
    // this allocation, to be be sped up, should be done with a commandBuffer
    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = commandPool;
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
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

void HelloTriangleApplication::mapVertices(VkDeviceSize size, VkDeviceMemory& bufferMemory, const void* src) {

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

