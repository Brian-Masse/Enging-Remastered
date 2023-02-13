// // Libs
// #define GLFW_INCLUDE_VULKAN
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/glm.hpp>
// #include <mach-o/dyld.h>
// #include <GLFW/glfw3.h>
// #include <glm/gtc/matrix_transform.hpp>

// // std 
// #include <algorithm>
// #include <vector>
// #include <array>
// #include <stdexcept>
// #include <string>
// #include <iostream>
// #include <fstream>
// #include <sstream>


// #include "app.h"
// #include "universalConstructors.h"
// #include "vertexReader.h"

// using namespace std;

// void EngineRemastered::testFunc() {
//     cout << "this worked! Hello!!" << endl;
    
//     run();
// }


// void EngineRemastered::run() {
//     initWindow();

//     initVulkan();
//     // mainLoop();
//     // cleanup();
// }

// // MARK: Initialization
// void EngineRemastered::initWindow() {
//     // glfwInit();
//     // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // prevents it from initializing OpenGL
//     // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

//     // window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
//     // glfwSetWindowUserPointer(window, this);
//     // glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
// }

// void EngineRemastered::createInstance()
// {

//     cout << "Hello!" << endl;
//     // if (enableValidationLayers && !checkValidationLayerSupport()) { throw std::runtime_error("validation layers requested, but not available!"); }
    
//     // VkApplicationInfo appInfo{};
//     // appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//     // appInfo.pApplicationName = "Engine-Remastered";
//     // appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//     // appInfo.pEngineName = "No Engine";
//     // appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//     // appInfo.apiVersion = VK_API_VERSION_1_0;

//     // // tells the compiler the global extension / validation layers to use
//     // // global - applies to the entire program, not a specific computer
//     // VkInstanceCreateInfo createInfo{};
//     // createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//     // createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
//     // createInfo.pApplicationInfo = &appInfo;

//     // vector<const char *>  extensions = getRequiredExtensions();
//     // createInfo.enabledExtensionCount = extensions.size();
//     // createInfo.ppEnabledExtensionNames = extensions.data();

//     // VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

//     // if (enableValidationLayers) {
//     //     createInfo.enabledLayerCount = validationLayers.size();
//     //     createInfo.ppEnabledLayerNames = validationLayers.data();

//     //     populateDebugMessengerCreateInfo(debugCreateInfo);
//     //     createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
//     // }
//     // else { createInfo.enabledLayerCount = 0; }

//     // // pointer to information, custom allocator callbacks, and an instance to store the VKInstance (global class property)
//     // VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
//     // if (result != VK_SUCCESS)
//     // {
//     //     cout << result << endl;
//     //     throw std::runtime_error("failed to create instance!");
//     // }
// }

// void EngineRemastered:: initVulkan() {
//     createInstance();
//     // createSurface();
//     // setupMessenger();
//     // pickPhysicalDevice();
//     // createLogicalDevice();

//     // createSwapChain();
//     // createRenderPass();
//     // createDescriptorSetLayout();
//     // createGraphicsPipeline();
//     // createCommandPool();

//     // info.device = device;
//     // info.physicalDevice = physicalDevice;
//     // info.commandPool = commandPool;
//     // info.graphicsQueue = graphicsQueue;
//     // sampler = createImageSampler();
//     // createObjects();

//     // createImageViews();
//     // createDepthResources();
//     // createFrameBuffers();   

//     // createUniformBuffers();
//     // createDescriptorPools();
//     // createDescriptorSets();

//     // createCommandBuffers();
//     // createSyncFunctions();
// }