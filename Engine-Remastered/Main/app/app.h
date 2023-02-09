#ifndef APP_H
#define APP_H

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
#include "../objects/object.h"

using namespace std;
using namespace glm;



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

struct UniformConstantData {
    vec3 cameraPos;
};


class HelloTriangleApplication
{
public:
    void run();

private:
    const int WIDTH = 600;
    const int HEIGHT = 600;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    const vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    uint32_t currentFrame = 0;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger; 
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    GLFWwindow *window;
    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    UniformConstantData constantData = { {0, 0, -1} };

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
    vector<VkCommandBuffer> commandBuffers;

    void createCommandPool();
    void createCommandBuffers();
    VkCommandBuffer beginSingleTimeCommands();
    VkCommandBuffer endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    //MARK: Draw
    vector<VkSemaphore> imageAvailableSemaphores;
    vector<VkSemaphore> renderFinishedSemaphores;
    vector<VkFence> inFlightFences; 

    void createSyncFunctions();
    void drawFrame();

    //MARK: buffers
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    
    vector<VkBuffer> uniformBuffers;
    vector<VkDeviceMemory> uniformBuffersMemory;
    vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

    void createVertexBuffer();
    void createDescriptorSetLayout();
    void createUniformBuffers();
    void createDescriptorPools();
    void createDescriptorSets();

    void updateUniformBuffers(uint32_t currentImage);
    

    void createBuffer( VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory );
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties );
    void mapBuffer(VkDeviceSize size, VkDeviceMemory& bufferMemory, const void *);
    
    double temp = 0;

    
    vector<EngineObject::Vertex> vertices;
    vector<uint16_t> indices;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    void createIndexBuffer();
    void prepareVertices();

    //MARK: Image Sampling:

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler sampler;

    void createTextureImage();
    void createTextureImageView();
    void createImageSampler();
    void create2DImage( int width, int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage& image, VkDeviceMemory& memory );
    void transitionImageLayout( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout );
    void copyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height  );


    //MARK: DepthBuffer:
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    void createDepthResources();
    VkFormat findSupportedFormat( const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features );


    // MARK: Mainloop
    void mainLoop();

    // MARK: Cleanup
    void cleanupSwapChain();
    void cleanup();
};

#endif