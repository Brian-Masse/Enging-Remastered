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

    sampler = createImageSampler();
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

//returns a list of extensions
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

void EngineRemastered::createDeviceInfo() {
    info.device = device;
    info.physicalDevice = physicalDevice;
    info.commandPool = commandPool;
    info.graphicsQueue = graphicsQueue;
    info.MAX_FRAMES_IN_FLIGHT = MAX_FRAMES_IN_FLIGHT;
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
        if ( queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT ) { indicies.graphicsFamily = i; }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) { indicies.presentFamily = i; }

        if (indicies.isComplete()) { break;}

        i ++;
    }
    return indicies;
}

void EngineRemastered::createDescriptorSetMaterials() {

    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    // VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    // samplerLayoutBinding.binding = 1;
    // samplerLayoutBinding.descriptorCount = 1;
    // samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // samplerLayoutBinding.pImmutableSamplers = nullptr;
    // samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    vector< VkDescriptorSetLayoutBinding > bindings = { uboLayoutBinding };

    descriptorSetLayout = createDescriptorSetLayout(info, bindings);
    descriptorPool = createDescriptorPools(info, 1, 0);
    descriptorSets = allocateDescriptorSet(info, descriptorPool, descriptorSetLayout);

    VkDescriptorBufferInfo bufferInfo = {};
    // the actual buffer is asigned in the udpateDescriptorSet function
    // bufferInfo.buffer = uniformBuffers[0]; //memory has already been allocated in create uniform buffers
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof( UniformConstantData );

    updateDescriptorSet(info, descriptorSets, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo, nullptr, uniformBuffers);

        //specify binding, type, and info 

        // descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        // descriptorWrites[0].dstSet = descriptorSets[i];
        // descriptorWrites[0].dstBinding = 0;
        // descriptorWrites[0].dstArrayElement = 0;
        // descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // descriptorWrites[0].descriptorCount = 1;
        // descriptorWrites[0].pBufferInfo = &bufferInfo;

    // };


    // createDescriptorSets();

}

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

    createObject("cube.ply", "ramen.png", 1, 1, 1, 0, 0, 0, 1, 1, 1);
    createObject("icoSphere.ply", "base.png", 1, 1, 1, 1, 1, 0, 1, 1, 1);

}

VkSampler EngineRemastered::createImageSampler() {

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR; // oversampling
    samplerInfo.minFilter = VK_FILTER_LINEAR; // under-sampling

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // how the image is handled outside of the image bounds
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; // can be specifed on a per-vertex basis
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

    VkSampler sampler;
    VkResult result = vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
    if (result != VK_SUCCESS) { throw runtime_error( "Unable to create the texture Sampler" ); }
    return sampler;

}

void EngineRemastered::updateCamera( double x, double y, double z ) {
    constantData.cameraPos.x += x;
    constantData.cameraPos.y += y;
    constantData.cameraPos.z += z;
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

    vkDestroySampler(device, sampler, nullptr);

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