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

#include "ProxyFunctions/proxy.h"

//loading the binary SPV shader files in
static vector<char> readFile(const string& fileName) {
    ifstream file(fileName, ios::ate | ios::binary);

    if (!file.is_open()) { throw runtime_error( "Failed to open the file" );}

    //will read the file from the bottom, and create a buffer based on length;
    size_t fileSize = (size_t) file.tellg();
    vector<char> buffer(fileSize);

    file.seekg(0);
    file.read( buffer.data(), fileSize );

    file.close();
    return buffer;
}

using namespace std;

// MARK: Application
class EngineRemastered
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    // MARK: Constants / globals

    const int WIDTH = 600;
    const int HEIGHT = 600;

    const vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

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
    void initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // prevents it from initializing OpenGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
    }

    void initVulkan()
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
    }

    void createInstance()
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
    void createSurface() {
        VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
        if (result != VK_SUCCESS) { throw runtime_error("Failed to create the surface"); }
    }

    // MARK: Validation Layers
    bool checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        // checks requested layers are available
        vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName : validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerProperties.layerName, layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) { return false; }
        }
        return true;
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        //this lists the types of messages (severity and type) that this messenger can receive
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    void setupMessenger() {
        if (!enableValidationLayers) { return; } 
        
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
        if (result != VK_SUCCESS) { throw runtime_error("failed to setup debug Messenger"); }

    }

    //returns a list of extensions
    vector<const char *> getRequiredExtensions() {
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

    // Vulkan will call this function if something in a validation layer goes wrong
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, //VERBOSE, INFO, WARNING, ERROR (comparable)
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        cerr << "validation layer: " << pCallbackData->pMessage << endl;

        return VK_FALSE;
    }

    //MARK: Physical Device:
    void pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) { throw std::runtime_error("failed to find GPUs with Vulkan support!"); }

        vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable( device )) {
                physicalDevice = device;
                break;
            }
        }
        
        if (physicalDevice == VK_NULL_HANDLE) { throw runtime_error("Failed to find a compatible GPU for Vulkan!"); }
    }

    bool isDeviceSuitable( VkPhysicalDevice device ) {

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        //preform an special checks for the device using the above information
        QueueFamilyIndicies indicies = findQueueFamilies(device);

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails details = querySwapChainSupport(device);
            if ( !details.formats.empty() && !details.presentMode.empty() ) { swapChainAdequate = true; }
        }

        return indicies.isComplete() && extensionsSupported && swapChainAdequate;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, availableExtensions.data() );

        set<string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for ( const auto& extension : availableExtensions ) { requiredExtensions.erase( extension.extensionName ); }
        return requiredExtensions.empty();
    }

    //MARK: Swapchain
    const vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset"
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;        //num of images, size of images, 
        vector<VkSurfaceFormatKHR> formats;           //color space, pixel format
        vector<VkPresentModeKHR> presentMode;
    };

    SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device ) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize( formatCount );
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data() );
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (formatCount != 0) {
            details.presentMode.resize( presentModeCount );
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentMode.data());
        }
        return details;
    }

    //selecting an optimal surface Format for the swapchain
    VkSurfaceFormatKHR chooseSwapSurfaceFormat( const vector<VkSurfaceFormatKHR>& availableFormats ) {
        for (const auto& availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) { // prefer to work with 32 bit SRGB
                return availableFormat;
            }
        }
        return availableFormats[0]; // could be ranked, but this is just the first suitable one
    }

    VkPresentModeKHR chooseSurfacePresentMode( const vector<VkPresentModeKHR>& availablePresentModes ) {
        for ( const auto& availablePresentMode : availablePresentModes ) {
            if ( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return availablePresentMode; } // this is less energy efficient, but avoid latency
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) { return capabilities.currentExtent; }
        else {
            int width, height; 

            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void createSwapChain() {
        SwapChainSupportDetails details = querySwapChainSupport(physicalDevice);

        VkSurfaceFormatKHR format = chooseSwapSurfaceFormat(details.formats);
        VkPresentModeKHR presentMode = chooseSurfacePresentMode(details.presentMode);
        VkExtent2D swapExtent = chooseSwapExtent(details.capabilities);

        uint32_t imageCount = details.capabilities.minImageCount + 1;
        if (details.capabilities.maxImageCount > 0 && details.capabilities.maxImageCount) { imageCount = details.capabilities.maxImageCount; }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = format.format;
        createInfo.imageColorSpace = format.colorSpace;
        createInfo.imageExtent = swapExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndicies indices = findQueueFamilies(physicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        //specifies how images are shared between queue families
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // transparency in window systems
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE; //when new swapchains are created (window resize) it needs to reference the old one

        VkResult results = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
        if (results != VK_SUCCESS) { throw runtime_error( "Error creating the swapchain (cry)" ); }

        uint32_t swapChainImagesCount;
        vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, nullptr);
        swapChainImages.resize(swapChainImagesCount);
        vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, swapChainImages.data());

        swapChainImageFormat = format.format;
        swapChainExtent = swapExtent;
    } 

    void createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;


            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VkResult result = vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]);
            if (result != VK_SUCCESS) { throw runtime_error("failed to create an ImageView"); }
        }
    }


    //all actions are handled and put into queues, queues can only accept certain commands as a function of what queue family they are a part of
    // have to find what queue families are supported on each device
    struct QueueFamilyIndicies {
        //these could be the same families, adding logic to ensure this might improve performance
        optional<uint32_t> graphicsFamily;
        optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndicies findQueueFamilies( VkPhysicalDevice device ) {
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

    //MARK: Logical Device
    void createLogicalDevice() {
        QueueFamilyIndicies indices = findQueueFamilies(physicalDevice);

        //the queues we want to create in a specific family
        vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }


        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        VkPhysicalDeviceFeatures deviceFeatures{}; // specify the features you want

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = deviceExtensions.size(); //enables all the deviceExtensions (including swapchain)
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
        if (result != VK_SUCCESS) { throw runtime_error("Failed to create Logical Device"); }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

    }

    //MARK: Pipeline
    void createGraphicsPipeline() {

        auto vertShaderCode = readFile("GraphicsPipeline/SPVFiles/vert.spv");
        auto fragShaderCode = readFile("GraphicsPipeline/SPVFiles/frag.spv");

        //programmable functions

        VkShaderModule vertShaderModule = createShaderModule( vertShaderCode );
        VkShaderModule fragShaderModule = createShaderModule( fragShaderCode );

        VkPipelineShaderStageCreateInfo vertStageCreateInfo = {};
        vertStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertStageCreateInfo.module = vertShaderModule;
        vertStageCreateInfo.pName = "main";        //the entry point to the shader (function)

        VkPipelineShaderStageCreateInfo fragStageCreateInfo = {};
        fragStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragStageCreateInfo.module = fragShaderModule;
        fragStageCreateInfo.pName = "main";  

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertStageCreateInfo, fragStageCreateInfo };

        //fixed functions

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // how to connect all the vertices together
        inputAssembly.primitiveRestartEnable = VK_FALSE; // possible to break up lines / triangles depending on topology mod

        //Viewport & Scissor
        VkViewport viewport{}; // the window of the framebuffer to display, will copy / resize all pixels to fit
        viewport.x = 0.0f;      //both viewport and scissor can be dynamic 
        viewport.y = 0.0f;
        viewport.width = (float) swapChainExtent.width;
        viewport.height = (float) swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{}; // scissor rectangle will cut the pixels outside of it
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;

        vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineViewportStateCreateInfo viewPortCreateInfo = {};
        viewPortCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewPortCreateInfo.viewportCount = 1;
        viewPortCreateInfo.scissorCount = 1;

        //Rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;             //discard items outside of frame
        rasterizer.rasterizerDiscardEnable = VK_FALSE;      //don't ignore output of rasterizer
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;

        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        //MultiSampling
        VkPipelineMultisampleStateCreateInfo multisampling{}; // tool to help with anti-aliasing: requires feature to be enabled in the GPU
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        //Color blending
        VkPipelineColorBlendAttachmentState colorBlendAttachment{}; // these are the color blend modes per frame buffer
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional // this is an alpha blend combination
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending{}; // these are the color blend constants / specifications passed to per frame buffer calculations
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        VkResult layoutResult = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
        if (layoutResult != VK_SUCCESS) { throw runtime_error("Error creating the Pipeline Layout"); }


        VkGraphicsPipelineCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.stageCount = 2;
        createInfo.pStages = shaderStages;

        createInfo.pVertexInputState = &vertexInputInfo;
        createInfo.pInputAssemblyState = &inputAssembly;
        createInfo.pViewportState = &viewPortCreateInfo;
        createInfo.pRasterizationState = &rasterizer;
        createInfo.pMultisampleState = &multisampling;
        createInfo.pDepthStencilState = nullptr;
        createInfo.pColorBlendState = &colorBlending;
        createInfo.pDynamicState = &dynamicState;

        createInfo.layout = pipelineLayout;
        createInfo.renderPass = renderPass;
        createInfo.subpass = 0;

        createInfo.basePipelineHandle = VK_NULL_HANDLE; //for deriving one pipeline from another
        createInfo.basePipelineIndex = -1; //reference another pipeline when deriving. Must have VK_PIPELINE_CREATE_DERIVATIVE_BIT specified in flags 

        VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);
        if (result != VK_SUCCESS) { throw runtime_error("Failed to create the Graphics Pipeline"); }
        
        vkDestroyShaderModule(device, vertShaderModule, nullptr);
        vkDestroyShaderModule(device, fragShaderModule, nullptr);
    }

    VkShaderModule createShaderModule( const vector<char>& code ) {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
        if (result != VK_SUCCESS) { throw runtime_error("Unsuccessful creating a ShaderModule"); }

        return shaderModule;
    }

    //specify frame buffer attachments
    void createRenderPass() {
        //define attachments
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // what to do with the data before and after rendering
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //store the contents of the frame buffer after render
        
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; //same as the loadOP and StoreOP above for stencils
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;      // changes the layout in memory of textures and frame buffers
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // read from swapchain

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        //define subpasses that reference the attachments
        VkSubpassDescription ColorSubpass{};
        ColorSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        ColorSubpass.colorAttachmentCount = 1;
        ColorSubpass.pColorAttachments = &colorAttachmentRef; // the index of attachment here is referenced from the frag shader 'layout(location = 0) out vec4)'

        VkRenderPassCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &colorAttachment;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &ColorSubpass;

        VkResult result = vkCreateRenderPass(device, &createInfo, nullptr, &renderPass);
        if (result != VK_SUCCESS) { throw runtime_error("Error Creating the Render Pass"); }


    }

    // MARK: Mainloop
    void mainLoop()
    {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    // MARK: Cleanup
    void cleanup()
    {
        if (enableValidationLayers) { DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr); }
        for (auto& imageView : swapChainImageViews) { vkDestroyImageView(device, imageView, nullptr); }

        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroySwapchainKHR(device, swapChain, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);

        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

// MARK: Main
int main()
{
    EngineRemastered app;

    try { app.run(); }

    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}