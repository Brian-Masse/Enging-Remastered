#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>  // for error handling
#include <stdexcept> // for error handling
#include <cstdlib>   // EXIT SUCCESS / FAILURE macros
#include <vector>

#include "app.h"
#include "../ProxyFunctions/proxy.h"

using namespace std;

void HelloTriangleApplication::run()
{\
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

// MARK: Initialization
void HelloTriangleApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // prevents it from initializing OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

void HelloTriangleApplication:: initVulkan()
{
    createInstance();
    createSurface();
    setupMessenger();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFrameBuffers();
    createCommandPool();
    createVertexBuffer();
    createCommandBuffer();
    createSyncFunctions();
}

// MARK: Mainloop
void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        drawFrame();

    }

    vkDeviceWaitIdle(device);
}

//returns a list of extensions
vector<const char *> HelloTriangleApplication::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // need to add these extension to conform to the required VK_KHR_PORTABILITY_subset
    extensions.push_back("VK_KHR_portability_enumeration");
    extensions.push_back("VK_KHR_get_physical_device_properties2");
    extensions.push_back("VK_KHR_surface");

    if(enableValidationLayers) { extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ); }

    return extensions;
}

void HelloTriangleApplication::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport()) { throw std::runtime_error("validation layers requested, but not available!"); }
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Engine-Remastered";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // tells the compiler the global extension / validation layers to use
    // global - applies to the entire program, not a specific computer
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.pApplicationInfo = &appInfo;

    vector<const char *>  extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = validationLayers.size();
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else { createInfo.enabledLayerCount = 0; }

    // pointer to information, custom allocator callbacks, and an instance to store the VKInstance (global class property)
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        cout << result << endl;
        throw std::runtime_error("failed to create instance!");
    }
}

//MARK: Window
void HelloTriangleApplication::createSurface() {
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create the surface"); }
}

QueueFamilyIndicies HelloTriangleApplication::findQueueFamilies( VkPhysicalDevice device ) {
    QueueFamilyIndicies indicies;

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if ( queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT ) { indicies.graphicsFamily = i; }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) { indicies.presentFamily = i; }

        if (indicies.isComplete()) { break;}

        i ++;
    }
    return indicies;
}

// MARK: Cleanup
void HelloTriangleApplication::cleanup()
{
    if (enableValidationLayers) { DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr); }
    
    cleanupSwapChain();

    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
    vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}