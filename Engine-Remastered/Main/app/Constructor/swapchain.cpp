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
#include "../app.h"

//MARK: Swapchain

SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport( VkPhysicalDevice device ) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize( formatCount );
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data() );
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (formatCount != 0) {
        details.presentMode.resize( presentModeCount );
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentMode.data());
    }
    return details;
}

//selecting an optimal surface Format for the swapchain
VkSurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat( const vector<VkSurfaceFormatKHR>& availableFormats ) {
    for (const auto& availableFormat: availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) { // prefer to work with 32 bit SRGB
            return availableFormat;
        }
    }
    return availableFormats[0]; // could be ranked, but this is just the first suitable one
}

VkPresentModeKHR HelloTriangleApplication::chooseSurfacePresentMode( const vector<VkPresentModeKHR>& availablePresentModes ) {
    for ( const auto& availablePresentMode : availablePresentModes ) {
        if ( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return availablePresentMode; } // this is less energy efficient, but avoid latency
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) { return capabilities.currentExtent; }
    else {
        int width, height; 

        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void HelloTriangleApplication::createSwapChain() {
    SwapChainSupportDetails details = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR format = chooseSwapSurfaceFormat(details.formats);
    VkPresentModeKHR presentMode = chooseSurfacePresentMode(details.presentMode);
    VkExtent2D swapExtent = chooseSwapExtent(details.capabilities);

    uint32_t imageCount = details.capabilities.minImageCount;
    if (details.capabilities.maxImageCount > 0 && details.capabilities.maxImageCount) { imageCount = details.capabilities.maxImageCount; }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    // should be imageCount
    createInfo.minImageCount = MAX_FRAMES_IN_FLIGHT;
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = swapExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndicies indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    //specifies how images are shared between queue families
    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // transparency in window systems
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE; //when new swapchains are created (window resize) it needs to reference the old one

    VkResult results = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
    if (results != VK_SUCCESS) { throw runtime_error( "Error creating the swapchain (cry)" ); }

    uint32_t swapChainImagesCount;
    vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, nullptr);
    swapChainImages.resize(swapChainImagesCount);
    vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, swapChainImages.data());

    swapChainImageFormat = format.format;
    swapChainExtent = swapExtent;
} 

void HelloTriangleApplication::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {

        swapChainImageViews[i] = createImageView( swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT );
    }
}

void HelloTriangleApplication::createFrameBuffers() {
    swapChainFramebuffers.resize( swapChainImages.size() );

    for (size_t i = 0; i < swapChainImages.size(); i++) {

        std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
        if (result != VK_SUCCESS) { throw runtime_error( "Failed to create a framebuffer" ); }
    }
}

void HelloTriangleApplication::recreateSwapChain() {

    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createFrameBuffers();

    createUniformBuffers();
    createDescriptorSets();

}

void HelloTriangleApplication::cleanupSwapChain() {

    for (auto framebuffer : swapChainFramebuffers) {  vkDestroyFramebuffer(device, framebuffer, nullptr); }
    for (auto& imageView : swapChainImageViews) { vkDestroyImageView(device, imageView, nullptr); }
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer( device, uniformBuffers[i], nullptr );
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
    
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}