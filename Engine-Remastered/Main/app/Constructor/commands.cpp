// Libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std 
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <fstream>

// dependencies
#include "../app.h"
 
// command pools manage the memory for command buffers: command buffers are allocated from command pools
void EngineRemastered::createCommandPool() {
    QueueFamilyIndicies queueFamilyIndicies = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // allows commandbuffers to reset and rerecorded every frame individually
    createInfo.queueFamilyIndex = queueFamilyIndicies.graphicsFamily.value(); //each commandPool can only create a command buffer with a single type of queue. This is for drawing

    VkResult result = vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create CommandPool"); }

}

void EngineRemastered::createCommandBuffers() {

    commandBuffers.resize( MAX_FRAMES_IN_FLIGHT );

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //can be put into the queue directly; cannot be called form other command buffers
    allocInfo.commandBufferCount = commandBuffers.size();

    VkResult result = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create CommandBuffer"); }

}

void EngineRemastered::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

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

    array<VkClearValue, 2> clearValues = {};
    clearValues[0].color = {{0.1f, 0.05f, 0.05f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassBeginInfo.clearValueCount = clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();

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

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
        pipelineLayout, 0, 1, 
        &descriptorSets[currentFrame], 0, nullptr);
    // vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( PushConstantData ), &pushData.cameraPos );

    // for using vertexBuffer
    // vkCmdDraw(commandBuffer, vertices.size(), 1, 0, 0);
    vkCmdDrawIndexed(commandBuffer, indices.size(), 1, 0, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    VkResult endResult = vkEndCommandBuffer(commandBuffer);
    if (endResult != VK_SUCCESS) { throw runtime_error("Failed to record the commandBuffer!"); }
    
}