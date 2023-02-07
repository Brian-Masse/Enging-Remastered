#ifndef APP_H

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
#include <limits>
#include <optional>
#include <set>

// additions
#include <cstdlib> // EXIT SUCCESS / FAILURE macros
#include <cstring>
#include <fstream>
#include <sstream>

#include "proxy.h"
#include "Buffers/vertexBuffer.h"
#include "../objects/object.h"

using namespace std;



//included Structs
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;        //num of images, size of images, 
    vector<VkSurfaceFormatKHR> formats;           //color space, pixel format
    vector<VkPresentModeKHR> presentMode;
};

//all actions are handled and put into queues, queues can only accept certain commands as a function of what queue family they are a part of
// have to find what queue families are supported on each device
struct QueueFamilyIndicies {
    //these could be the same families, adding logic to ensure this might improve performance
    optional<uint32_t> graphicsFamily;
    optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct PushConstantData {
    vec3 cameraPos;
};


class HelloTriangleApplication
{
public:
    void run();

private:
    const int WIDTH = 600;
    const int HEIGHT = 600;

    const vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger; 
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    GLFWwindow *window;
    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    PushConstantData pushData = { {0, 0, -1} };

    // MARK: Initialization
    void initWindow();
    void initVulkan();
    static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->frameBufferResized = true;
    }

    void createInstance();
    void createSurface();

    //MARK: Validation
    bool checkValidationLayerSupport();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupMessenger();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, //VERBOSE, INFO, WARNING, ERROR (comparable)
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
            printf("");

            cerr << "validation layer: " << pCallbackData->pMessage << endl;
            return VK_FALSE;
        }

    vector<const char *> getRequiredExtensions();

    //MARK: PhysicalDevice
    void pickPhysicalDevice();
    bool isDeviceSuitable( VkPhysicalDevice device );
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    //MARK: Swapchain
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    vector<VkImage> swapChainImages;
    vector<VkImageView> swapChainImageViews;
    vector<VkFramebuffer> swapChainFramebuffers;
    bool frameBufferResized = false;

    const vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset"
    };

    SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device );
    VkSurfaceFormatKHR chooseSwapSurfaceFormat( const vector<VkSurfaceFormatKHR>& availableFormats );
    VkPresentModeKHR chooseSurfacePresentMode( const vector<VkPresentModeKHR>& availablePresentModes );
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSwapChain();
    void recreateSwapChain();

    void createImageViews();
    void createFrameBuffers();

    QueueFamilyIndicies findQueueFamilies( VkPhysicalDevice device );

    //MARK: Logical Device
    void createLogicalDevice();

    //MARK: Pipeline
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout; // this can specify uniform values, which can be passed into shaders at run time to change their behavior.. they must be pre-specified 
    VkPipeline pipeline;

    void createGraphicsPipeline();
    VkShaderModule createShaderModule( const vector<char>& code );
    void createRenderPass();

    //MARK: Commands
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    void createCommandPool();
    void createCommandBuffer();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    //MARK: Draw
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence; 

    void createSyncFunctions();
    void drawFrame();

    //MARK: VertexBuffer
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    void createVertexBuffer();
    void createBuffer( VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory );
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties );

    void mapVertices(VkDeviceSize size, VkDeviceMemory& bufferMemory, const void *);
    
    double temp = 0;

    // std::vector<Vertex> vertices;
    //  = {
    //     {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
    //     {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
    //     {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
    //     {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}
    // };

    vector< EngineObject::Vertex> vertices{
 
      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
 
      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
 
      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
 
      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
 
      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
 
      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
 
    };

    void prepareVertices();


    //MARK: index buffer
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    void createIndexBuffer();

    vector<uint16_t> indices;
    // = {
    //     0, 1, 2, 3,
    //     4, 5, 6, 7,
    //     8, 9, 10, 11,
    //     12, 13, 14, 15,
    //     16, 17, 18, 19,
    //     20, 21, 22, 23,
    // };


    // MARK: Mainloop
    void mainLoop();

    // MARK: Cleanup
    void cleanupSwapChain();
    void cleanup();
};

#endif