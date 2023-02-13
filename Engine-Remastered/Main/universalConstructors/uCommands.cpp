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

#include "universalConstructors.h"

using namespace std;

VkCommandBuffer beginSingleTimeCommands(DeviceInfo info) {

    VkCommandBuffer buffer;
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = info.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //can be put into the queue directly; cannot be called form other command buffers
    allocInfo.commandBufferCount = 1;

    VkResult allocResult = vkAllocateCommandBuffers(info.device, &allocInfo, &buffer);
    if (allocResult != VK_SUCCESS) { throw runtime_error("Failed to create CommandBuffer"); }


    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;

    VkResult result = vkBeginCommandBuffer(buffer, &beginInfo);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to begin command buffer recording"); }

    return buffer;
}

VkCommandBuffer endSingleTimeCommands(DeviceInfo info, VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(commandBuffer);

    vkQueueSubmit(info.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(info.graphicsQueue);

    vkFreeCommandBuffers(info.device, info.commandPool, 1, &(commandBuffer));

    return commandBuffer;
}