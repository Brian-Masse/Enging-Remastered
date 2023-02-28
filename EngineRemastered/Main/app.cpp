// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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
#include "universalConstructors.h"
#include "vertexReader.h"

using namespace std;

void EngineRemastered::initialization() {
    initWindow();
    initVulkan();
}

void EngineRemastered::run() {
    mainLoop();
    cleanup();
}

// MARK: Initialization
void EngineRemastered::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // prevents it from initializing OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

void EngineRemastered:: initVulkan() {
    createInstance();
    createSurface();
    setupMessenger();
    pickPhysicalDevice();
    createLogicalDevice();

    createSwapChain();
    createRenderPass();
    createCommandPool();
    createDeviceInfo();
    createUniformBuffers();

    createObjects();

    createDescriptorSetMaterials();
    createGraphicsPipeline();

    createImageViews();
    createDepthResources();
    createFrameBuffers();   

    createCommandBuffers();
    createSyncFunctions();
}

// MARK: Mainloop
void EngineRemastered::mainLoop()
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void EngineRemastered::setPath(string path) {
    this->path = path;
}

//returns a list of extensions

void EngineRemastered::createDeviceInfo() {
    info.device = device;
    info.physicalDevice = physicalDevice;
    info.commandPool = commandPool;
    info.graphicsQueue = graphicsQueue;
    info.MAX_FRAMES_IN_FLIGHT = MAX_FRAMES_IN_FLIGHT;
    info.path = path;
}

void EngineRemastered::createDescriptorSetMaterials() {

    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    vector< VkDescriptorSetLayoutBinding > bindings = { uboLayoutBinding };

    descriptorSetLayout = createDescriptorSetLayout(info, bindings);
    descriptorPool = createDescriptorPools(info, 1, 0);
    descriptorSets = allocateDescriptorSet(info, descriptorPool, descriptorSetLayout);

    // the actual buffer is assigned in the updateDescriptorSet function
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof( UniformConstantData );

    updateDescriptorSet(info, descriptorSets, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo, nullptr, &uniformBuffers);

}

//MARK: Objects
void EngineRemastered::createObject(string name, string texture, double sx, double sy, double sz, double tx, double ty, double tz, double r, double g, double b) {

    EngineObject obj;
    obj.info = info;
    obj.fileName = name;
    obj.texture = texture;
    obj.init();

    mat4 translateMatrix = mat4{1.0f};
    translateMatrix[3][0] = tx;
    translateMatrix[3][1] = ty;
    translateMatrix[3][2] = tz;

    mat4 scaleMatrix = mat4(1.0f);
    scaleMatrix[0][0] = sx;
    scaleMatrix[1][1] = sy;
    scaleMatrix[2][2] = sz;

    obj.transform.translation = translateMatrix;
    obj.transform.scale = scaleMatrix;

    obj.transform.color = vec3( r, g, b );

    objects.push_back(obj);

}

void EngineRemastered::createObjects() {
    createObject( "cube.ply", "base2.png", 0, 0, 0, 0, 0, 0, 1, 1, 1 );
    // createObject("monkey.ply", "base2.png", 1, 1, 1, -0.5, -0.5, 0, 1, 1, 1);
    // createObject("monkey.ply", "base.png", 1, 1, 1, 1, 1, 0, 1, 1, 1);
}

void EngineRemastered::updateCamera( double x, double y, double z ) {
    constantData.cameraPos.x += x;
    constantData.cameraPos.y += y;
    constantData.cameraPos.z += z;
}

//MARK: Uniform Buffers

void EngineRemastered::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof( UniformConstantData );

    uniformBuffers.resize( MAX_FRAMES_IN_FLIGHT );
    uniformBuffersMemory.resize( MAX_FRAMES_IN_FLIGHT );

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i ++) {
        createBuffer(info, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        uniformBuffers[i], 
        uniformBuffersMemory[i]);
    }
}

void EngineRemastered::updateUniformBuffers(uint32_t currentImage) {
    // do any frame by frame updates to the uniform buffer memory here!
    
    double moveSpeed = 0.05;

    if ( glfwGetKey(window, GLFW_KEY_UP) ) { updateCamera(0, -moveSpeed, 0); }
    if ( glfwGetKey(window, GLFW_KEY_DOWN) ) { updateCamera(0, moveSpeed, 0); }
    if ( glfwGetKey(window, GLFW_KEY_LEFT) ) { updateCamera(-moveSpeed, 0, 0); }
    if ( glfwGetKey(window, GLFW_KEY_RIGHT) ) { updateCamera(moveSpeed,0, 0); }
    if ( glfwGetKey(window, GLFW_KEY_W) ) { updateCamera(0, 0, moveSpeed); }
    if ( glfwGetKey(window, GLFW_KEY_S) ) { updateCamera(0, 0, -moveSpeed); }

    mapBuffer(info, sizeof(UniformConstantData), uniformBuffersMemory[currentImage], &constantData);

}

//MARK: Window
void EngineRemastered::createSurface() {
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    if (result != VK_SUCCESS) { throw runtime_error("Failed to create the surface"); }
}

QueueFamilyIndicies EngineRemastered::findQueueFamilies( VkPhysicalDevice device ) {
    QueueFamilyIndicies indicies;

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) { indicies.graphicsFamily = i; }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) { indicies.presentFamily = i; }

        if (indicies.isComplete()) { break;}

        i ++;
    }
    return indicies;
}

vector<const char *> EngineRemastered::getRequiredExtensions() {
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

void EngineRemastered::createInstance()
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

// MARK: Cleanup
void EngineRemastered::cleanup()
{
    if (enableValidationLayers) { DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr); }
    
    cleanupSwapChain();

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    for (auto& object : objects) { object.cleanup(); }

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