//
// Created by Diego S. Seabra on 14/03/22.
//

#include "VulkanRenderer.h"
#include "Window.h"
#include "../profiling/Logger.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
CVulkanRendererImpl* mImplementation = nullptr;

/*
 * Pointer to Implementation
 */
void CVulkanRenderer::Init()
{
    mImplementation = new CVulkanRendererImpl;
}

void CVulkanRenderer::Draw()
{
    // TODO: Implement drawing
}

void CVulkanRenderer::Shutdown()
{
    delete mImplementation;
}

/*
 * Constructor
 */
CVulkanRendererImpl::CVulkanRendererImpl()
{
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();
    CreateCommandPool();
    CreateDepthResources();
    CreateFramebuffers();
    CreateTextureImage();
    CreateImageView();
    CreateTextureSampler();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateCommandBuffers();
    CreateSyncObjects();
}

/*
 * Destructor
 */
CVulkanRendererImpl::~CVulkanRendererImpl()
{
    vkDestroyDevice(vkLogicalDevice, nullptr);

    vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
    vkDestroyInstance(vkInstance, nullptr);
}

/*
 * Auxiliary Methods
 */
bool CVulkanRendererImpl::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName: validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties: availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

QueueFamilyIndices CVulkanRendererImpl::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices{};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily: queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        // look for a queue family that has the capability of presenting to our window surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vkSurface, &presentSupport);

        if (presentSupport)
            indices.presentFamily = i;

        if (indices.isComplete())
            break;

        i++;
    }

    // Assign index to queue families that could be found
    return indices;
}

bool CVulkanRendererImpl::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionsCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionsCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails CVulkanRendererImpl::QuerySwapChainSupport(VkPhysicalDevice device)
{
    // querying basic surface capabilities
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vkSurface, &details.capabilities);

    // querying the supported surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        // resize to hold all the available formats
        std::cout << "# of available formats: " << formatCount << std::endl;
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface, &formatCount, details.formats.data());
    }

    // querying the supported presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        // resize to hold all the available present modes
        std::cout << "# of present modes: " << presentModeCount << std::endl;
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool CVulkanRendererImpl::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAdequate = false;

    // we only check for swap chain support AFTER checking that the extension is available
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported &&
           swapChainAdequate && supportedFeatures.samplerAnisotropy;
}
/*
 * Methods
 */
void CVulkanRendererImpl::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Elixir Game Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "Elixir Game Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0 ,1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::cout << "extensions count: " << glfwExtensionCount << std::endl;
    std::cout << *glfwExtensions << std::endl;

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (enableValidationLayers && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available");
    }

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "Available extensions:\n";

    for (const auto& extension: extensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create Vulkan instance");
    }
}

void CVulkanRendererImpl::SetupDebugMessenger()
{

}

void CVulkanRendererImpl::CreateSurface()
{
    if (glfwCreateWindowSurface(vkInstance, CWindow::GetWindow(), nullptr, &vkSurface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }
}

void CVulkanRendererImpl::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPU's with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    for (const auto& device: devices)
    {
        if (IsDeviceSuitable(device))
        {
            vkPhysicalDevice = device;
            break;
        }
    }

    if (vkPhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU");
    }

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &props);

    uint32_t vulkanVersion = props.apiVersion;

    std::cout << "GPU: " << props.deviceName << std::endl;
    std::cout << "Vulkan Version: " << VK_API_VERSION_MAJOR(vulkanVersion) << "." <<
              VK_API_VERSION_MINOR(vulkanVersion) << "." << VK_API_VERSION_PATCH(vulkanVersion) << std::endl;
}

void CVulkanRendererImpl::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphicsFamily.value(), indices.presentFamily.value()
    };

    // assigning a priority queue (0.0f -> 1.0f)
    float queuePriority = 1.0f;
    for (uint32_t queueFamily: uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    // creating the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // retro compatibility with older implementations
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else
    {
        createInfo.enabledLayerCount = 0;
    }

    // we're now ready to instantiate the logical device
    if (vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkLogicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(vkLogicalDevice, indices.graphicsFamily.value(), 0, &vkGraphicsQueue);
    vkGetDeviceQueue(vkLogicalDevice, indices.presentFamily.value(), 0, &vkPresentQueue);
}

void CVulkanRendererImpl::CreateSwapChain()
{

}

void CVulkanRendererImpl::CreateImageViews()
{

}

void CVulkanRendererImpl::CreateRenderPass()
{

}

void CVulkanRendererImpl::CreateDescriptorSetLayout()
{

}

void CVulkanRendererImpl::CreateGraphicsPipeline()
{

}

void CVulkanRendererImpl::CreateCommandPool()
{

}

void CVulkanRendererImpl::CreateDepthResources()
{

}

void CVulkanRendererImpl::CreateFramebuffers()
{

}

void CVulkanRendererImpl::CreateTextureImage()
{

}

void CVulkanRendererImpl::CreateImageView()
{

}

void CVulkanRendererImpl::CreateTextureSampler()
{

}

void CVulkanRendererImpl::CreateVertexBuffer()
{

}

void CVulkanRendererImpl::CreateIndexBuffer()
{

}

void CVulkanRendererImpl::CreateUniformBuffers()
{

}

void CVulkanRendererImpl::CreateDescriptorPool()
{

}

void CVulkanRendererImpl::CreateDescriptorSets()
{

}

void CVulkanRendererImpl::CreateCommandBuffers()
{

}

void CVulkanRendererImpl::CreateSyncObjects()
{

}


