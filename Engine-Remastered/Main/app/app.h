#ifndef APP_H

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

#include "proxy.h"
#include "Buffers/vertexBuffer.h"

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

    std::vector<Vertex> vertices;
    // = {
    //     {{0.0f, -0.5f, 1.0}, {1.0f, 0.0f, 0.0f}},
    //     {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    //     {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    //     {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    // };

    void prepareVertices();


    //MARK: index buffer
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    void createIndexBuffer();

    const vector<uint16_t> indices = {
        4, 0, 1, 2, 3,
        4, 4, 5, 6, 7,
        4, 8, 9, 10, 11,
        4, 12, 13, 14, 15,
        4, 16, 17, 18, 19,
        4, 20, 21, 22, 23,
    };
    
    // MARK: Mainloop
    void mainLoop();

    // MARK: Cleanup
    void cleanupSwapChain();
    void cleanup();
};

#endif