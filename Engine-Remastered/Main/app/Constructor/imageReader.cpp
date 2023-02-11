// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

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

#include "../app.h"
#include "../proxy.h"

void HelloTriangleApplication::createTextureImage() {

    int textureWidth, textureHeight, textureChannels;

    string pathToDir = "Textures/";
    string fullPath = getAbsoluteDirectory(pathToDir, "base.png");

    const char* url =  fullPath.c_str();

    stbi_uc* pixels = stbi_load(url, &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha  );
    VkDeviceSize imageSize = textureWidth * textureHeight * 4;

    if (!pixels) { throw runtime_error("Failed to open Image!"); }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VkBufferUsageFlags stagingBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkMemoryPropertyFlags stagingMemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    createBuffer(imageSize, stagingBufferUsage, stagingMemoryProperties, stagingBuffer, stagingBufferMemory);
    mapBuffer(imageSize, stagingBufferMemory, pixels);
    stbi_image_free(pixels);

    create2DImage( textureWidth, textureHeight, 
        VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,  
        textureImage, textureImageMemory );

    //copy the staging buffer to VkImage

    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, textureImage, textureWidth, textureHeight);
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    textureImageView = createImageView( textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
}

VkImageView HelloTriangleApplication::createImageView(VkImage& image, VkFormat format, VkImageAspectFlagBits aspectMask) {

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectMask;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &imageView);
    if (result != VK_SUCCESS) { throw runtime_error( "Unable to Create Texture Image View" ); } 

    return imageView;
}

void HelloTriangleApplication::createImageSampler() {

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
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

    VkResult result = vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
    if (result != VK_SUCCESS) { throw runtime_error( "Unable to create the texture Sampler" ); }

}

//have to transition the image layout before copying the buffer memory into an Image object
void HelloTriangleApplication::transitionImageLayout( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout ) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    //used to transition image layouts
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout; // can be undefined if not interested in preserving the old content of the image 
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // for transferring ownership between queue families: (for exclusive ownership models )
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // 

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStages;
    VkPipelineStageFlags destinationStages;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0; // nothing runs before it
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // this waits on the barrier

        sourceStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; // run as soon as possible
        destinationStages = VK_PIPELINE_STAGE_TRANSFER_BIT; //wait during a transistion stage

    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStages = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

    }else { throw runtime_error( "Unsupported transition type!" ); }

    barrier.srcAccessMask = 0; //tasks to do before the barrier
    barrier.dstAccessMask = 0; // tasks to wait on the barrier

    // second parameter is the pipeline stage that tasks run before the barrier should be run during
    // third parameter is the pipeline stage that tasks waiting for the barrier should wait during
    vkCmdPipelineBarrier(commandBuffer, sourceStages, destinationStages, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    endSingleTimeCommands(commandBuffer);
}

void HelloTriangleApplication::copyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height  ) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    // this is the region of the buffer to copy to a certain to part of the iamge
    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0; //space between pixel data
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}


void HelloTriangleApplication::create2DImage( int width, int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage& image, VkDeviceMemory& memory ) {

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;

    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    VkResult result = vkCreateImage(device, &imageInfo, nullptr, &image);
    if (result != VK_SUCCESS) { throw runtime_error( "failed to create Image!" ); }

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(device, image, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VkResult allocResult = vkAllocateMemory(device, &allocInfo, nullptr, &memory);
    if (allocResult != VK_SUCCESS) { throw runtime_error( "Failed to allocate Memory!" ); }

    vkBindImageMemory(device, image, memory, 0);

}