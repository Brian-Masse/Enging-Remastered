#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>  // for error handling
#include <stdexcept> // for error handling
#include <cstdlib>   // EXIT SUCCESS / FAILURE macros
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>

using namespace std;

//MARK: Proxy Functions

// //loading the binary SPV shader files in
// vector<char> readFile(const string& fileName) {
//     ifstream file(fileName, ios::ate | ios::binary);

//     if (!file.is_open()) { throw runtime_error( "Failed to open the file" );}

//     //will read the file from the bottom, and create a buffer based on length;
//     size_t fileSize = (size_t) file.tellg();
//     vector<char> buffer(fileSize);

//     file.seekg(0);
//     file.read( buffer.data(), fileSize );

//     file.close();
//     return buffer;
// }

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
