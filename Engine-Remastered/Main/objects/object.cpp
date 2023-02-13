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
    createVertexBuffer();
    createIndexBuffer();
}

void EngineObject::cleanup() {
    VkDevice device = info.device;

    vkFreeMemory(device, textureImageMemory, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroyImage(device, textureImage, nullptr);

    vkFreeMemory(device, vertexBufferMemory, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkDestroyBuffer(device, indexBuffer, nullptr);
}

void EngineObject::extractVertices() {

    EngineObject::BufferInformation information = extractInformation(fileName);

    this->vertices = information.vertexBuffer;
    this->indices = information.indexBuffer;
}

//MARK: Image

void EngineObject::createTextureMaterials() {
    createTextureImage();
}

void EngineObject::createTextureImage() {

    int textureWidth, textureHeight, textureChannels;

    string pathToDir = "Textures/";
    string fullPath = getAbsoluteDirectory(pathToDir, "ramen.png");

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

void EngineObject::bind(VkCommandBuffer commandBuffer) {

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void EngineObject::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {

    vkCmdPushConstants(commandBuffer, pipelineLayout, 
        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( TransformPushConstants ),
        &transform );

    // vkCmdDraw(commandBuffer, vertices.size(), 1, 0, 0);
    vkCmdDrawIndexed(commandBuffer, indices.size(), 1, 0, 0, 0);
}

