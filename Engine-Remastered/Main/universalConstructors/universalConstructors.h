#ifndef UNIVERSAL_CONSTRUCTORS_H
#define UNIVERSAL_CONSTRUCTORS_H

// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION
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

using namespace std;

struct DeviceInfo {

    VkDevice device;
    VkPhysicalDevice physicalDevice;

    VkCommandPool commandPool;
    VkQueue graphicsQueue;

};


//MARK: Image Functions

VkImageView createImageView( DeviceInfo info, VkImage& image, VkFormat format, VkImageAspectFlagBits aspectMask);

void transitionImageLayout( DeviceInfo info, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout );

void copyBufferToImage( DeviceInfo info, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height  );

void create2DImage( DeviceInfo info, int width, int height, VkFormat format, VkImageTiling tiling, unsigned int usage, VkImage_T*& image, VkDeviceMemory_T*& memory );

//MARK: Buffers

uint32_t findMemoryType(DeviceInfo info, uint32_t typeFilter, VkMemoryPropertyFlags properties);

void mapBuffer( DeviceInfo info, VkDeviceSize size, VkDeviceMemory& bufferMemory, const void* src);

void createBuffer( DeviceInfo info, VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory );

void copyBuffer(DeviceInfo info, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

//MARK: Commands

VkCommandBuffer beginSingleTimeCommands(DeviceInfo info);

VkCommandBuffer endSingleTimeCommands(DeviceInfo info, VkCommandBuffer commandBuffer);

//MARK: Proxy

string getAbsoluteDirectory( string pathFromEntry, string fileName );

VkResult CreateDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);

void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);

#endif