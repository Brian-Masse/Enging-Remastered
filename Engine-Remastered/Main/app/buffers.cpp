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

void updateDescriptorSet(DeviceInfo info, vector<VkDescriptorSet> descriptorSets, int binding, VkDescriptorType type, VkDescriptorBufferInfo* bufferInfo, VkDescriptorImageInfo* imageInfo, vector<VkBuffer> buffers) {
    


    for (size_t i = 0; i < info.MAX_FRAMES_IN_FLIGHT; i++) {
        
        // const size_t count = objects.size();
        // const int uniformBuffersCount = 1;

        // array<VkWriteDescriptorSet, 2> descriptorWrites{};
        vector<VkWriteDescriptorSet> descriptorWrites = {};
        descriptorWrites.resize( 1 );

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = binding;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = type;
        descriptorWrites[0].descriptorCount = 1;

        if (bufferInfo != nullptr) { 
            bufferInfo->buffer = buffers[i];
            descriptorWrites[0].pBufferInfo = bufferInfo; 
        }
        else if (imageInfo != nullptr) { descriptorWrites[0].pImageInfo = imageInfo; }



        // for (int f = 0; f < count; f++) {

        //     VkDescriptorImageInfo imageInfo{};
        //     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        //     imageInfo.imageView = objects[f].textureImageView;
        //     imageInfo.sampler = sampler;

        //     descriptorWrites[f + uniformBuffersCount].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        //     descriptorWrites[f + uniformBuffersCount].dstSet = descriptorSets[i];
        //     descriptorWrites[f + uniformBuffersCount].dstBinding = 1;
        //     descriptorWrites[f + uniformBuffersCount].dstArrayElement = 0;
        //     descriptorWrites[f + uniformBuffersCount].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //     descriptorWrites[f + uniformBuffersCount].descriptorCount = 1;
        //     descriptorWrites[f + uniformBuffersCount].pImageInfo = &imageInfo;
        // }

        vkUpdateDescriptorSets(info.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
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