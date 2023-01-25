#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <fstream>

#include "../app.h"
 
// command pools manage the memory for command buffers: command buffers are allocated from command pools
void HelloTriangleApplication::createCommandPool() {
    QueueFamilyIndicies queueFamilyIndicies = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // allows commandbuffers to reset and rerecorded every frame individually
    createInfo.queueFamilyIndex = queueFamilyIndicies.graphicsFamily.value(); //each commandPool can only create a command buffer with a single type of queue. This is for drawing

    VkResult result = vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create CommandPool"); }

}

void HelloTriangleApplication::createCommandBuffer() {

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //can be put into the queue directly; cannot be called form other command buffers
    allocInfo.commandBufferCount = 1;

    VkResult result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create CommandBuffer"); }

}

void HelloTriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;

    VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to begin command buffer recording"); }

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = swapChainFramebuffers[imageIndex]; //framebuffer for the swapchain image we want to draw to

    renderPassBeginInfo.renderArea.offset = {0,0};
    renderPassBeginInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}}; //the color when the renderPass clears the frame
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    //because viewport and scissor were marked as dynamic, need to specify them in the commandBuffer
    VkViewport viewPort = {};
    viewPort.x = 0.0f;
    viewPort.y = 0.0f;
    viewPort.width = swapChainExtent.width;
    viewPort.height = swapChainExtent.height;
    viewPort.minDepth = 0.0f;
    viewPort.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewPort);

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    VkResult endResult = vkEndCommandBuffer(commandBuffer);
    if (endResult != VK_SUCCESS) { throw runtime_error("Failed to record the commandBuffer!"); }
    
}