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

using namespace std;

//MARK: Proxy Functions

string getAbsoluteDirectory( string pathFromEntry, string fileName ) {
    ostringstream pathName;
    char buf [PATH_MAX];
    uint32_t bufSize = PATH_MAX;
    if ( _NSGetExecutablePath(buf, &bufSize) != 0 ) { throw runtime_error("error getting executable path"); }

    uint32_t bufLength = strlen(buf);
    *(buf + bufLength - 3) = '\0'; 

    pathName << buf << "../Engine-Remastered/" << pathFromEntry << fileName;

    return pathName.str();
}

//The function to create a messenger is in an extension, this function safely retrieves it
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) { return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

    } else { return VK_ERROR_EXTENSION_NOT_PRESENT; }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}