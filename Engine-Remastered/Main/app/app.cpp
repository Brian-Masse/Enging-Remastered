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

#include "app.h"
#include "../VertexHandler/vertexReader.h"
// #include "../objects/object.h"

using namespace std;

void HelloTriangleApplication::run()
{
    initWindow();
    prepareVertices();
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
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFrameBuffers();
    createCommandPool();
    createTextureImage();
    createTextureImageView();
    createImageSampler();
    createDepthResources();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPools();
    createDescriptorSets();
    createCommandBuffer( commandBuffer );
    createSyncFunctions();
}

void HelloTriangleApplication::prepareVertices() {

    EngineObject::BufferInformation information = extractInformation("cube.ply");

    // for ( auto& index: information.indexBuffer ) {
    //     auto& pos = information.vertexBuffer[index].pos;
    //     auto& normal = information.vertexBuffer[index].normal;

    //     cout << index << ":    " << pos.x << ", " << pos.y << ", " << pos.z << endl;
    //     cout << normal.x << ", " << normal.y << ", " << normal.z << endl;
    // }

    this->vertices = information.vertexBuffer;
    this->indices = information.indexBuffer;

    // this ->indices = {
    //     1, 0, 2, 3,
    //     5, 4, 6, 7,
    //     9, 8, 10, 11,
    //     // 13, 12, 14, 15,
    //     // 16, 17, 18, 19,
    //     // 20, 21, 22, 23,
    // };

}

// MARK: Mainloop
void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window)) {

        // temp++;

        // double x = cos( temp / 100 ) / 2;
        // double y = sin( temp / 100 ) / 2;
        // double z = (cos( temp / 100 ) / 2) + 1.5;

        // constantData.cameraPos.x = x;
        // constantData.cameraPos.y = y;
        // pushData.cameraPos.z = z;

        // vertices[0].pos[0] = y;

        // VkDeviceSize size = sizeof( vertices[0] ) * vertices.size();

        // VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        // VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;


        // VkBuffer stagingBuffer;
        // VkDeviceMemory stagingMemory;
        // createBuffer(size, stagingUsageFlags, stagingMemoryFlags, stagingBuffer, stagingMemory);
        // mapVertices(size, stagingMemory, vertices.data());

        // copyBuffer(stagingBuffer, vertexBuffer, size);  //copy the vertex data from CPU-accessible memory to optimized GPU only memory

        // vkDestroyBuffer(device, stagingBuffer, nullptr);
        // vkFreeMemory(device, stagingMemory, nullptr);
        
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

    vkDestroySampler(device, sampler, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);
    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
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