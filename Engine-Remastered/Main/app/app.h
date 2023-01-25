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

#include "../ProxyFunctions/proxy.h"

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
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout; // this can specify uniform values, which can be passed into shaders at run time to change their behavior.. they must be pre-specified 
    VkPipeline pipeline;

    vector<VkImage> swapChainImages;
    vector<VkImageView> swapChainImageViews;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    // MARK: Initialization
    void initWindow();
    void initVulkan();

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

            cerr << "validation layer: " << pCallbackData->pMessage << endl;
            return VK_FALSE;
        }

    vector<const char *> getRequiredExtensions();

    //MARK: PhysicalDevice
    void pickPhysicalDevice();
    bool isDeviceSuitable( VkPhysicalDevice device );
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    //MARK: Swapchain
    const vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset"
    };

    SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device );
    VkSurfaceFormatKHR chooseSwapSurfaceFormat( const vector<VkSurfaceFormatKHR>& availableFormats );
    VkPresentModeKHR chooseSurfacePresentMode( const vector<VkPresentModeKHR>& availablePresentModes );
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSwapChain();
    void createImageViews();

    QueueFamilyIndicies findQueueFamilies( VkPhysicalDevice device );

    //MARK: Logical Device
    void createLogicalDevice();

    //MARK: Pipeline
    void createGraphicsPipeline();
    VkShaderModule createShaderModule( const vector<char>& code );
    void createRenderPass();
    
    // MARK: Mainloop
    void mainLoop();

    // MARK: Cleanup
    void cleanup();
};