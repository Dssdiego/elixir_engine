//
// Created by Diego S. Seabra on 14/03/22.
//

#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif

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

VkInstance CVulkanRenderer::GetInstance()
{
    return mImplementation->vkInstance;
}

VkPhysicalDevice CVulkanRenderer::GetPhysicalDevice()
{
    return mImplementation->vkPhysicalDevice;
}

VkDevice CVulkanRenderer::GetLogicalDevice()
{
    return mImplementation->vkLogicalDevice;
}

VkQueue CVulkanRenderer::GetGraphicsQueue()
{
    return mImplementation->vkGraphicsQueue;
}

VkRenderPass CVulkanRenderer::GetRenderPass()
{
    // TODO: Implement
    return nullptr;
}

VkDescriptorPool CVulkanRenderer::GetDescriptorPool()
{
    return mImplementation->vkDescriptorPool;
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
    CleanupSwapChain();

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

VkSurfaceFormatKHR CVulkanRendererImpl::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    // using SRGB color format
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR CVulkanRendererImpl::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    // checking if triple buffering is available
    for (const auto& availablePresentMode: availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    // if triple buffering is not available, use Vulkan's default swap chain queue (aka VSync)
    return VK_PRESENT_MODE_FIFO_KHR;
}

// Swap Extent = resolution of the swap chain images
VkExtent2D CVulkanRendererImpl::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(CWindow::GetWindow(), &width, &height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width =
                std::max(capabilities.minImageExtent.width,
                         std::min(capabilities.maxImageExtent.width,
                                  actualExtent.width));

        actualExtent.height =
                std::max(capabilities.minImageExtent.height,
                         std::min(capabilities.maxImageExtent.height,
                                  actualExtent.height));

        return actualExtent;
    }
}

VkImageView CVulkanRendererImpl::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.format = format;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(vkLogicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view");
    }

    return imageView;
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
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vkPhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // one more image to have "room" for more processing
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        // making sure we don't exceed the maximum number of images in the swap chain
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    std::cout << "Minimum image count in the swap chain: " << imageCount << std::endl;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkSurface; // tying our surface to the swap chain
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // the amount of layers each image consists of (always 1 unless we are developing a stereoscopic 3D app)
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // image operations, like color attachment (our case), post-processing, etc

    QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    // specifying how to handle swap chain images that will be used across multiple queue families (graphics and presentation)
    // in our case, we'll be drawing on the images in the swap chain from the graphics queue and then
    //    submitting them on the presentation queue
    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // optional
        createInfo.pQueueFamilyIndices = nullptr; // optional
    }

    // we don't want any pre-transformation on the images for now
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    // compositeAlpha = blending with other windows in the window system. So we will ignore the alpha channel (aka opaque)
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE; // we don't care for pixels that are obscured (ex.: another window in front of our own)
    createInfo.oldSwapchain = VK_NULL_HANDLE; // for now we assume that we'll be creating only one swap chain.
    // NOTE: Only one swap chain is not the best option because we'd like the screen to be resized, so in the future we'll probably recreate the swap chain ;)

    // effectively creating the swap chain
    if (vkCreateSwapchainKHR(vkLogicalDevice, &createInfo, nullptr, &vkSwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create the swap chain");
    }

    // retrieve swap chain images
    vkGetSwapchainImagesKHR(vkLogicalDevice, vkSwapChain, &imageCount, nullptr);
    vkSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkLogicalDevice, vkSwapChain, &imageCount, vkSwapChainImages.data());

    vkSwapChainImageFormat = surfaceFormat.format;
    vkSwapChainExtent = extent;
}

void CVulkanRendererImpl::CreateImageViews()
{
    // resizing the list to fit all of the image views
    vkSwapChainImageViews.resize(vkSwapChainImages.size());

    for (size_t i = 0; i < vkSwapChainImages.size(); i++)
    {
        vkSwapChainImageViews[i] = CreateImageView(vkSwapChainImages[i], vkSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    std::cout << "# of image views created: " << vkSwapChainImageViews.size() << std::endl;
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
    // descriptor pool to hold our drawings as well as ImGui's drawings
    VkDescriptorPoolSize poolSizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = std::size(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(vkLogicalDevice, &poolInfo, nullptr, &vkDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to create descriptor pool");
    }

    // vulkan-tutorial's "original" code
//    std::array<VkDescriptorPoolSize, 2> poolSizes{};
//    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//    poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
//    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//    poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
//
//    VkDescriptorPoolCreateInfo poolInfo{};
//    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//    poolInfo.pPoolSizes = poolSizes.data();
//    poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size()); // maximum number of descriptors that may be allocated
//
//    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
//    {
//        throw std::runtime_error("failed to create descriptor pool");
//    }
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

void CVulkanRendererImpl::CleanupSwapChain()
{
    for (auto imageView : vkSwapChainImageViews)
    {
        vkDestroyImageView(vkLogicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(vkLogicalDevice, vkSwapChain, nullptr);

    vkDestroyDescriptorPool(vkLogicalDevice, vkDescriptorPool, nullptr);
}


