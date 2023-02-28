// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

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

#include "object.h"
#include "universalConstructors.h"
#include "vertexReader.h"

using namespace std;

void EngineObject::init() {
    extractVertices();
    createTextureMaterials();   
    createDescriptorSetMaterials();
    createVertexBuffer();
    createIndexBuffer();
}

void EngineObject::cleanup() {
    VkDevice device = info.device;

    vkDestroySampler(device, sampler, nullptr);
    vkDestroyDescriptorSetLayout(info.device, descriptorSetLayout, nullptr);
    vkDestroyDescriptorPool(info.device, descriptorPool, nullptr);

    vkFreeMemory(device, textureImageMemory, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroyImage(device, textureImage, nullptr);

    vkFreeMemory(device, vertexBufferMemory, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkDestroyBuffer(device, indexBuffer, nullptr);
}

void EngineObject::extractVertices() {

    EngineObject::BufferInformation information = extractInformation(info, fileName, isAbsolute);

    this->vertices = information.vertexBuffer;
    this->indices = information.indexBuffer;
}

//MARK: Image

void EngineObject::createTextureMaterials() {
    createImageSampler();
    createTextureImage();
}

void EngineObject::createTextureImage() {

    int textureWidth, textureHeight, textureChannels;

    string pathToDir = "Textures/";
    string fullPath = getAbsoluteDirectory(info.path, pathToDir, texture);

    const char* url =  fullPath.c_str();

    stbi_uc* pixels = stbi_load(url, &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha  );
    VkDeviceSize imageSize = textureWidth * textureHeight * 4;

    if (!pixels) { throw runtime_error("Failed to open Image!"); }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VkBufferUsageFlags stagingBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkMemoryPropertyFlags stagingMemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    createBuffer(info, imageSize, stagingBufferUsage, stagingMemoryProperties, stagingBuffer, stagingBufferMemory);
    mapBuffer(info, imageSize, stagingBufferMemory, pixels);
    stbi_image_free(pixels);

    create2DImage( info, textureWidth, textureHeight, 
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,  
        textureImage, textureImageMemory );

    //copy the staging buffer to VkImage

    transitionImageLayout(info, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(info, stagingBuffer, textureImage, textureWidth, textureHeight);
    transitionImageLayout(info, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(info.device, stagingBuffer, nullptr);
    vkFreeMemory(info.device, stagingBufferMemory, nullptr);

    textureImageView = createImageView( info, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
}

void EngineObject::createImageSampler() {

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR; // oversampling
    samplerInfo.minFilter = VK_FILTER_LINEAR; // under-sampling

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // how the image is handled outside of the image bounds
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; // can be specifed on a per-vertex basis
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(info.physicalDevice, &properties);

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

    VkResult result = vkCreateSampler(info.device, &samplerInfo, nullptr, &sampler);
    if (result != VK_SUCCESS) { throw runtime_error( "Unable to create the texture Sampler" ); }

}

void EngineObject::createDescriptorSetMaterials() {

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    vector< VkDescriptorSetLayoutBinding > bindings = { samplerLayoutBinding };

    descriptorSetLayout = createDescriptorSetLayout(info, bindings);

    descriptorPool = createDescriptorPools(info, 0, 1);
    descriptorSets = allocateDescriptorSet(info, descriptorPool, descriptorSetLayout);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = textureImageView;
    imageInfo.sampler = sampler;

    updateDescriptorSet(info, descriptorSets, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, nullptr, &imageInfo, nullptr);
}

void EngineObject::bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, int currentFrame) {

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
        pipelineLayout, 1, 1, 
        &descriptorSets[currentFrame], 0, nullptr);
}

void EngineObject::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {

    vkCmdPushConstants(commandBuffer, pipelineLayout, 
        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( TransformPushConstants ),
        &transform );

    // vkCmdDraw(commandBuffer, vertices.size(), 1, 0, 0);
    vkCmdDrawIndexed(commandBuffer, indices.size(), 1, 0, 0, 0);
}

