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

//MARK: DescriptorSets

VkDescriptorPool createDescriptorPools(DeviceInfo info, int bufferCount, int imageCount) {
    vector<VkDescriptorPoolSize> poolSizes{};
    poolSizes.resize( bufferCount + imageCount );

    for (int i = 0; i < bufferCount; i++) {
        poolSizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[i].descriptorCount = info.MAX_FRAMES_IN_FLIGHT;
    }

    for (int i = 0; i < imageCount; i++) {
        poolSizes[i + bufferCount].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[i + bufferCount].descriptorCount = info.MAX_FRAMES_IN_FLIGHT;
    }

    VkDescriptorPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = poolSizes.size();
    createInfo.pPoolSizes = poolSizes.data(); // create a descriptor set for every frame
    createInfo.maxSets = info.MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPool descriptorPool;

    VkResult result = vkCreateDescriptorPool(info.device, &createInfo, nullptr, &descriptorPool);
    if (result != VK_SUCCESS) { throw runtime_error( "Unable to Create Descriptor Pool" ); }

    return descriptorPool;
}

VkDescriptorSetLayout createDescriptorSetLayout( DeviceInfo info, vector<VkDescriptorSetLayoutBinding> bindings) {

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VkDescriptorSetLayout descriptorSetLayout;
    VkResult result = vkCreateDescriptorSetLayout(info.device, &layoutInfo, nullptr, &descriptorSetLayout);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create Descriptor Set Layout!"); }

    return descriptorSetLayout;
}

vector<VkDescriptorSet> allocateDescriptorSet(DeviceInfo info, VkDescriptorPool pool, VkDescriptorSetLayout& descriptorSetLayout) {

    vector<VkDescriptorSetLayout> layouts(info.MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = info.MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data(); 

    vector<VkDescriptorSet> descriptorSets;
    descriptorSets.resize( info.MAX_FRAMES_IN_FLIGHT );

    VkResult result = vkAllocateDescriptorSets(info.device, &allocInfo, descriptorSets.data());
    if (result != VK_SUCCESS) { throw runtime_error( "Failed to allocate the Descriptor Sets!" ); }

    return descriptorSets;
}

void updateDescriptorSet(DeviceInfo info, vector<VkDescriptorSet>& descriptorSets, int binding, VkDescriptorType type, VkDescriptorBufferInfo* bufferInfo, VkDescriptorImageInfo* imageInfo, vector<VkBuffer>* buffers) {
    
    for (size_t i = 0; i < info.MAX_FRAMES_IN_FLIGHT; i++) {
        
        vector<VkWriteDescriptorSet> descriptorWrites = {};
        descriptorWrites.resize( 1 );

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = binding;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = type;
        descriptorWrites[0].descriptorCount = 1;

        if (bufferInfo != nullptr) { 
            bufferInfo->buffer = (*buffers)[i];
            descriptorWrites[0].pBufferInfo = bufferInfo; 
        }
        else if (imageInfo != nullptr) { descriptorWrites[0].pImageInfo = imageInfo; }

        vkUpdateDescriptorSets(info.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}




//MARK: Buffers:

void createBuffer( DeviceInfo info, VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory ) {
    //buffers are space in memory for data to be stored for the GPU to read
    // Memory must be managed explicitly
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = flags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //will only be used by the graphics queue, so it does not need to be shared among other queue families

    VkResult result = vkCreateBuffer(info.device, &bufferInfo, nullptr, &buffer);
    if (result != VK_SUCCESS) { throw runtime_error( "Failed to create the Buffer" ); }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(info.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize =  memRequirements.size;
    allocateInfo.memoryTypeIndex = findMemoryType(info, memRequirements.memoryTypeBits, memFlags);

    VkResult memResult = vkAllocateMemory(info.device, &allocateInfo, nullptr, &bufferMemory);
    if (memResult != VK_SUCCESS) { throw runtime_error( "Failed to Allocate Memory" ); }
    
    vkBindBufferMemory(info.device, buffer, bufferMemory, 0); 
}

void mapBuffer(DeviceInfo info, VkDeviceSize size, VkDeviceMemory& bufferMemory, const void* src) {

    void* data;
    vkMapMemory(info.device, bufferMemory, 0, size, 0, &data);
    memcpy(data, src, (size_t) size);
    vkUnmapMemory(info.device, bufferMemory);
}

uint32_t findMemoryType(DeviceInfo info, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(info.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { return i; }
    }

    throw runtime_error("Failed to find suitable Memory Type!");
}

void copyBuffer(DeviceInfo info, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    //need to temporarily allocate a command buffer for transferring vertex data between buffers
    // this allocation, to be be sped up, should be done with a commandBuffer
    VkCommandBuffer commandBuffer;
    commandBuffer = beginSingleTimeCommands(info);

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(info, commandBuffer);
}