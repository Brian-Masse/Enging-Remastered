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

#include "app.h"
#include "universalConstructors.h"

using namespace std;
using namespace glm;

void EngineRemastered::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();


    VkResult result = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create Descriptor Set Layout!"); }
}

void EngineRemastered::createDescriptorSets() {

    vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data(); 

    descriptorSets.resize( MAX_FRAMES_IN_FLIGHT );
    VkResult result = vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data());
    if (result != VK_SUCCESS) { throw runtime_error( "Failed to allocate the Descriptor Sets!" ); }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i]; //memory has already been allocated in create uniform buffers
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof( UniformConstantData );

        const size_t count = objects.size();
        const int uniformBuffersCount = 1;

        // array<VkWriteDescriptorSet, 2> descriptorWrites{};
        vector<VkWriteDescriptorSet> descriptorWrites = {};
        descriptorWrites.resize( count + uniformBuffersCount );

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        for (int f = 0; f < count; f++) {

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = objects[f].textureImageView;
            imageInfo.sampler = sampler;

            descriptorWrites[f + uniformBuffersCount].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[f + uniformBuffersCount].dstSet = descriptorSets[i];
            descriptorWrites[f + uniformBuffersCount].dstBinding = 1;
            descriptorWrites[f + uniformBuffersCount].dstArrayElement = 0;
            descriptorWrites[f + uniformBuffersCount].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[f + uniformBuffersCount].descriptorCount = 1;
            descriptorWrites[f + uniformBuffersCount].pImageInfo = &imageInfo;
        }

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void EngineRemastered::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof( UniformConstantData );

    uniformBuffers.resize( MAX_FRAMES_IN_FLIGHT );
    uniformBuffersMemory.resize( MAX_FRAMES_IN_FLIGHT );

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i ++) {
        createBuffer(info, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        uniformBuffers[i], 
        uniformBuffersMemory[i]);
    }
}



void EngineRemastered::updateUniformBuffers(uint32_t currentImage) {
    // do any frame by frame updates to the uniform buffer memory here!

    // temp++;

    // double x = cos( temp / 100 ) / 2;
    // double y = sin( temp / 100 ) / 2;
    // double z = (cos( temp / 100 ) / 2) + 1.5;

    // constantData.cameraPos.x = x;
    // constantData.cameraPos.y = y;

    double moveSpeed = 0.05;

    if ( glfwGetKey(window, GLFW_KEY_UP) ) { updateCamera(0, -moveSpeed, 0); }
    if ( glfwGetKey(window, GLFW_KEY_DOWN) ) { updateCamera(0, moveSpeed, 0); }
    if ( glfwGetKey(window, GLFW_KEY_LEFT) ) { updateCamera(-moveSpeed, 0, 0); }
    if ( glfwGetKey(window, GLFW_KEY_RIGHT) ) { updateCamera(moveSpeed,0, 0); }
    if ( glfwGetKey(window, GLFW_KEY_W) ) { updateCamera(0, 0, moveSpeed); }
    if ( glfwGetKey(window, GLFW_KEY_S) ) { updateCamera(0, 0, -moveSpeed); }

    void* data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(UniformConstantData), 0, &data);
    memcpy(data, &constantData, sizeof(UniformConstantData));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}