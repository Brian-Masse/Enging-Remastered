// Libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>

// std 
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// dependencies
#include "app.h"
#include "universalConstructors.h"

void EngineRemastered::createDepthResources() {
    
    VkFormat depthFormat = findDepthFormat();
    bool hasStencilComponent = (depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) || (depthFormat == VK_FORMAT_D24_UNORM_S8_UINT);

    create2DImage( info, swapChainExtent.width, swapChainExtent.height, depthFormat, 
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        depthImage, depthImageMemory );
    
    depthImageView = createImageView( info, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT );
    
}

VkFormat EngineRemastered:: findDepthFormat() {
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat EngineRemastered:: findSupportedFormat( const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features ) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw runtime_error( "could not find suitable depth format" );

}

