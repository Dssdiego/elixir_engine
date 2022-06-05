//
// Created by Diego S. Seabra on 14/03/22.
//

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <array>
#endif

#include <cstring>
#include "VulkanRenderer.h"
#include "Window.h"
#include "../profiling/Logger.h"
#include "Vertex.h"
#include "Shader.h"
#include "VulkanContext.h"
#include "../config.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                                 \
			std::cout << "Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)


// Vulkan context
SVulkanContext vkContext;

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
CVulkanRendererImpl* mVulkanRendererImpl = nullptr;

/*
 * Pointer to Implementation
 */
void CVulkanRenderer::Init()
{
    mVulkanRendererImpl = new CVulkanRendererImpl;
}

void CVulkanRenderer::Draw()
{
    // TODO: Implement drawing
}

void CVulkanRenderer::Shutdown()
{
    delete mVulkanRendererImpl;
}

/*
 * Constructor
 */
CVulkanRendererImpl::CVulkanRendererImpl()
{
    // Create the instance
    CreateInstance();

    // Set up debugger
    SetupDebugMessenger();

    // Create the presentation surface
    CreateSurface();

//    EnumeratePhysicalDevices();

    // Find queue family/indices supporting graphics and present
    PickPhysicalDevice();

    // Create logical device and queues
    CreateLogicalDeviceAndQueues();

    // Create semaphores
    CreateSemaphores();

    // Create command pool
    CreateCommandPool();

    // Create command buffer
    CreateCommandBuffer();

    // Create swap chain
    CreateSwapChain();

    // Create render pass
    CreateRenderPass();

    // Create render targets
//    CreateRenderTargets();

    // Create pipeline cache
//    CreatePipelineCache();

    // Create image views
    CreateImageViews();

    // Create depth resources
    CreateDepthResources();

    // Create frame buffers
    CreateFramebuffers();

    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();
    CreateTextureImage();
    CreateTextureSampler();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
}

/*
 * Destructor
 */
CVulkanRendererImpl::~CVulkanRendererImpl()
{
    CleanupSwapChain();

    vkDestroyImageView(vkContext.logicalDevice, vkDepthImageView, nullptr);
    vkDestroyImage(vkContext.logicalDevice, vkDepthImage, nullptr);
    vkFreeMemory(vkContext.logicalDevice, vkDepthImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(vkContext.logicalDevice, vkDescriptorSetLayout, nullptr);

    for (size_t i = 0; i < NUM_FRAME_DATA; i++)
    {
        vkDestroySemaphore(vkContext.logicalDevice, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vkContext.logicalDevice, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(vkContext.logicalDevice, commandBufferFences[i], nullptr);
        vkFreeCommandBuffers(vkContext.logicalDevice, vkCommandPool, 1, &vkContext.commandBuffers[i]);
    }

    vkDestroyCommandPool(vkContext.logicalDevice, vkCommandPool, nullptr);

    vkDestroyDevice(vkContext.logicalDevice, nullptr);

    vkDestroySurfaceKHR(vkContext.instance, vkSurface, nullptr);
    vkDestroyInstance(vkContext.instance, nullptr);
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

    return extensionsSupported &&
           swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

VkSurfaceFormatKHR CVulkanRendererImpl::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    VkSurfaceFormatKHR result;

    // if vulkan returned an unknown format, then we just force what we want.
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        result.format = VK_FORMAT_B8G8R8A8_UNORM;
        result.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return result;
    }

    // favor 32 bit rgba and srgb nonlinear colorspace
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // if all else fails, just return what's available
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

void CVulkanRendererImpl::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                 VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                 VkDeviceMemory& imageMemory)
{

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1; // how many texels are on each axis of the image
    imageInfo.mipLevels = 1; // not using mipmapping for now
    imageInfo.arrayLayers = 1; // texture is not an array
    imageInfo.format = format; // same as the pixels in the buffer
    imageInfo.tiling = tiling; // texels are laid out in an implementation defined order for optimal access
    // NOTE: tiling can't be changed at a later time, layout can
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT; // related to multisampling
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // the image will only be used by one queue family
    imageInfo.flags = 0; // optional

    VK_CHECK(vkCreateImage(vkContext.logicalDevice, &imageInfo, nullptr, &image));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkContext.logicalDevice, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(vkContext.logicalDevice, &allocInfo, nullptr, &imageMemory));

    vkBindImageMemory(vkContext.logicalDevice, image, imageMemory, 0);
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
    VK_CHECK(vkCreateImageView(vkContext.logicalDevice, &viewInfo, nullptr, &imageView));

    return imageView;
}

VkFormat CVulkanRendererImpl::FindDepthFormat()
{
    return FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat CVulkanRendererImpl::FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                                  VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(vkContext.physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    CLogger::Error("failed to find supported format", "");
    throw std::runtime_error("failed to find supported format");
}

uint32_t CVulkanRendererImpl::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    // querying info about the available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkContext.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find a suitable memory type");
}

VkShaderModule CVulkanRendererImpl::CreateShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    VK_CHECK(vkCreateShaderModule(vkContext.logicalDevice, &createInfo, nullptr, &shaderModule));

    return shaderModule;
}

/*
 * Methods
 */
void CVulkanRendererImpl::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = ENGINE_NAME; // TODO: Game name should go here!
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = ENGINE_NAME;
    appInfo.engineVersion = VK_MAKE_VERSION(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION);

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
        CLogger::Error("validation layers requested, but not available", "");
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

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &vkContext.instance));

    CLogger::Debug("Instance created");
}

void CVulkanRendererImpl::SetupDebugMessenger()
{
    // TODO: Implement debug messenger
}

void CVulkanRendererImpl::CreateSurface()
{
    VK_CHECK(glfwCreateWindowSurface(vkContext.instance, CWindow::GetWindow(), nullptr, &vkSurface));

    CLogger::Debug("Surface created");
}

void CVulkanRendererImpl::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkContext.instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        CLogger::Error("failed to find GPU's with Vulkan support", "");
        throw std::runtime_error("failed to find GPU's with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkContext.instance, &deviceCount, devices.data());

    for (const auto& device: devices)
    {
        if (IsDeviceSuitable(device))
        {
            vkContext.physicalDevice = device;
            break;
        }
    }

    if (vkContext.physicalDevice == VK_NULL_HANDLE)
    {
        CLogger::Error("failed to find a suitable GPU", "");
        throw std::runtime_error("failed to find a suitable GPU");
    }

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(vkContext.physicalDevice, &props);

    // getting vulkan version
    uint32_t vulkanVersion = props.apiVersion;
    uint32_t vulkanVMajor = VK_API_VERSION_MAJOR(vulkanVersion);
    uint32_t vulkanVMinor = VK_API_VERSION_MINOR(vulkanVersion);
    uint32_t vulkanVPatch = VK_API_VERSION_PATCH(vulkanVersion);

    std::stringstream ssVulkanVersion;
    ssVulkanVersion << "Vulkan Version: " << vulkanVMajor << "." << vulkanVMinor << "." << vulkanVPatch;

    CLogger::Info("GPU: " + std::string(props.deviceName));
    CLogger::Info(ssVulkanVersion.str());

    CLogger::Debug("Picked physical device");
}

void CVulkanRendererImpl::CreateLogicalDeviceAndQueues()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
            &vkContext.graphicsFamilyIdx, &vkContext.presentFamilyIdx
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
    VK_CHECK(vkCreateDevice(vkContext.physicalDevice, &createInfo, nullptr, &vkContext.logicalDevice));

    // get queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkContext.physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkContext.physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily: queueFamilies)
    {
        // getting the graphics queue family index
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            vkContext.graphicsFamilyIdx = i;

        // look for a queue family that has the capability of presenting to our window surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkContext.physicalDevice, i, vkSurface, &presentSupport);

        // getting the present queue family index
        if (presentSupport)
            vkContext.presentFamilyIdx = i;

        i++;
    }

    vkGetDeviceQueue(vkContext.logicalDevice, vkContext.graphicsFamilyIdx, 0, &vkContext.graphicsQueue);
    vkGetDeviceQueue(vkContext.logicalDevice, vkContext.presentFamilyIdx, 0, &vkContext.presentQueue);

    CLogger::Debug("Created logical device and queues");
}

void CVulkanRendererImpl::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vkContext.physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    vkContext.swapChainImageCount = swapChainSupport.capabilities.minImageCount + 1; // one more image to have "room" for more processing
    if (swapChainSupport.capabilities.maxImageCount > 0 && vkContext.swapChainImageCount > swapChainSupport.capabilities.maxImageCount)
    {
        // making sure we don't exceed the maximum number of images in the swap chain
        vkContext.swapChainImageCount = swapChainSupport.capabilities.maxImageCount;
    }
    std::cout << "Minimum image count in the swap chain: " << vkContext.swapChainImageCount << std::endl;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkSurface; // tying our surface to the swap chain
    createInfo.minImageCount = vkContext.swapChainImageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // the amount of layers each image consists of (always 1 unless we are developing a stereoscopic 3D app)

    // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - This is a color image we're rendering into
    // VK_IMAGE_USAGE_TRANSFER_SRC_BIT - We'll be copying the image somewhere (screenshot, postProcess)
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    uint32_t queueFamilyIndices[] = {vkContext.graphicsFamilyIdx, vkContext.presentFamilyIdx};

    // specifying how to handle swap chain images that will be used across multiple queue families (graphics and presentation)
    // in our case, we'll be drawing on the images in the swap chain from the graphics queue and then
    //    submitting them on the presentation queue
    if (vkContext.graphicsFamilyIdx != vkContext.presentFamilyIdx)
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
    VK_CHECK(vkCreateSwapchainKHR(vkContext.logicalDevice, &createInfo, nullptr, &vkSwapChain));

    // retrieve swap chain images from the logical device
    vkGetSwapchainImagesKHR(vkContext.logicalDevice, vkSwapChain, &vkContext.swapChainImageCount, nullptr);
    vkSwapChainImages.resize(vkContext.swapChainImageCount);
    vkGetSwapchainImagesKHR(vkContext.logicalDevice, vkSwapChain, &vkContext.swapChainImageCount, vkSwapChainImages.data());

    vkSwapChainImageFormat = surfaceFormat.format;
    vkSwapChainExtent = extent;

    CLogger::Debug("Swap chain created");
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
    // for now, just a single color buffer attachment represented by one of the images from the swap chain
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vkSwapChainImageFormat; // matching the format of the swap chain images
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // matching the format of the swap chain images
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // clear the framebuffer before drawing a new frame
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // rendered contents will be stored in memory
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // doing nothing with stencil buffers for now, so we don't care what is in the buffer
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // doing nothing with stencil buffers for now, so we don't care what is in the buffer
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // layout of the image before the render pass begins
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // layout to automatically transition the image to when the render pass finishes

    // we'll stick to a single subpass for now (so index 0)
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // depth
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // wait for the swap chain to finish reading before we can access it
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    // effectively creating the render pass
    VK_CHECK(vkCreateRenderPass(vkContext.logicalDevice, &renderPassInfo, nullptr, &vkContext.renderPass));

    CLogger::Debug("Render pass created");
}

void CVulkanRendererImpl::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // optional

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VK_CHECK(vkCreateDescriptorSetLayout(vkContext.logicalDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout));

    CLogger::Debug("Descriptor set layout created");
}

void CVulkanRendererImpl::CreateGraphicsPipeline()
{
    // SECTION: 1. Shaders
    auto vertShaderCode = CShader::ReadFile("assets/shaders/vert.spv");
    auto fragShaderCode = CShader::ReadFile("assets/shaders/frag.spv");

    // creating shader module
    VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

    // assigning shaders to a pipeline stage
    // vertex
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main"; // this allows us to combine multiple shaders in a single shader module (using multiple entry points)

    // fragment
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    // SECTION: 2. Vertex Input and Assembly
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; // type of vertex data that will be passed to the vertex shader
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    // bindings: spacing between data and whether the data is per-vertex or per-instance
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

    // attribute descriptions: type of attributes passed to the vertex shader, which binding and at which offset
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // vertex input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // because we want to draw a triangle for now
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // SECTION: 3. Viewport
    //  region of the framebuffer that the output will be rendered to
    // will probably be always from (0,0) to (width, height)
    // NOTE: the size of the swap chain and its images may differ from the width and height of the window
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) vkSwapChainExtent.width;
    viewport.height = (float) vkSwapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // SECTION: 4. Scissor
    //   scissor rectangles define in which regions pixels will actually be stored
    //   any pyxel outside will be discarded by the rasterizer (it's like a filter)
    // for now, the scissor rectangle will be the size of the viewport
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = vkSwapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1; // REVIEW: More than one viewport makes a split screen game?
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // SECTION: 5. Rasterizer
    //  takes the geometry that is shaped by the vertices from the vertex shader and turns it into fragments to be
    //  colored by the fragment shader
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE; // setting this to VK_TRUE disabled any output to the framebuffer

    // polygonMode: determines how fragments are generated for geometry, in this case it will fill the area of the polygon with fragments
    // NOTE: if we wanted to draw wireframes, for example, this would be changed to VK_POLYGON_MODE_LINE (making this will require enabling a GPU feature)
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; // for now, we'll always cull the back face
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // order for faces to be considered front-facing (in our case is counter clockwise because of MVP Y-flip in the shader)
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // optional
    rasterizer.depthBiasClamp = 0.0f; // optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // optional

    // SECTION: 6. Multisampling
    //  one of the ways to perform anti-aliasing
    //  for now, multisampling will be kept DISABLED
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // optional
    multisampling.pSampleMask = nullptr; // optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // optional
    multisampling.alphaToOneEnable = VK_FALSE; // optional

    // SECTION: 7. Depth and stencil testing
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE; // if the depth of new fragments should be compared to the depth buffer (to check if they should be discarded)
    depthStencil.depthWriteEnable = VK_TRUE; // if the new depth of fragments that pass the depth test should actually be written to the depth buffer (useful for drawing transparent objects)
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS; // right now, lower depth = closer
    depthStencil.depthBoundsTestEnable = VK_FALSE;
//        depthStencil.minDepthBounds = 0.0f; // optional
//        depthStencil.maxDepthBounds = 1.0f; // optional
    depthStencil.stencilTestEnable = VK_FALSE;

    // SECTION: 8. Color Blending
    //  combines the color returned from the fragment shader to the framebuffer color
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // optional
    colorBlending.blendConstants[1] = 0.0f; // optional
    colorBlending.blendConstants[2] = 0.0f; // optional
    colorBlending.blendConstants[3] = 0.0f; // optional

    // SECTION: 9. Pipeline Layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &vkDescriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount= 0; // optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // optional

    VK_CHECK(vkCreatePipelineLayout(vkContext.logicalDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout));

    // SECTION: 10. Graphics Pipeline Creation
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // optional
    pipelineInfo.layout = vkPipelineLayout;
    pipelineInfo.renderPass = vkContext.renderPass;
    pipelineInfo.subpass = 0; // not using subpasses for now
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
    pipelineInfo.basePipelineIndex = -1; // optional

    VK_CHECK(vkCreateGraphicsPipelines(vkContext.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline));

    // SECTION: 11. Cleaning up
    // shaders are assigned to a pipeline stage, so we can destroy the shader modules
    vkDestroyShaderModule(vkContext.logicalDevice, vertShaderModule, nullptr);
    vkDestroyShaderModule(vkContext.logicalDevice, fragShaderModule, nullptr);

    CLogger::Debug("Graphics pipeline created");
}

void CVulkanRendererImpl::CreateCommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // this allows the command buffer to be implicitly reset when 'vkBeginCommandBuffer' is called
    poolInfo.queueFamilyIndex = vkContext.graphicsFamilyIdx; // since we want to record the commands for drawing, we must use the graphics queue family

    VK_CHECK(vkCreateCommandPool(vkContext.logicalDevice, &poolInfo, nullptr, &vkCommandPool));

    CLogger::Debug("Command pool created");
}

void CVulkanRendererImpl::CreateDepthResources()
{
    VkFormat depthFormat = FindDepthFormat();

    CreateImage(vkSwapChainExtent.width, vkSwapChainExtent.height,
                depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkDepthImage,
                vkDepthImageMemory);

    vkDepthImageView = CreateImageView(vkDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void CVulkanRendererImpl::CreateFramebuffers()
{
    // resizing the container to hold all of the framebuffers
    vkSwapChainFrameBuffers.resize(vkSwapChainImageViews.size());

    // iterate through the image views and create framebuffers from them
    for (size_t i = 0; i < vkSwapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
                vkSwapChainImageViews[i],
                vkDepthImageView
        };

        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = vkContext.renderPass;
        frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        frameBufferInfo.pAttachments = attachments.data();
        frameBufferInfo.width = vkSwapChainExtent.width;
        frameBufferInfo.height = vkSwapChainExtent.height;
        frameBufferInfo.layers = 1; // out swap chain contains single images, so only 1 layer

        VK_CHECK(vkCreateFramebuffer(vkContext.logicalDevice, &frameBufferInfo, nullptr, &vkSwapChainFrameBuffers[i]));
    }

    CLogger::Debug("Framebuffers created");
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

    VK_CHECK(vkCreateDescriptorPool(vkContext.logicalDevice, &poolInfo, nullptr, &vkContext.descriptorPool));

    CLogger::Debug("Descriptor pool created");

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

void CVulkanRendererImpl::CreateCommandBuffer()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.commandBufferCount = NUM_FRAME_DATA; // 2 command buffers

    vkContext.commandBuffers.resize(NUM_FRAME_DATA);
    VK_CHECK(vkAllocateCommandBuffers(vkContext.logicalDevice, &allocInfo, vkContext.commandBuffers.data()));

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    commandBufferFences.resize(NUM_FRAME_DATA);
    for ( int i = 0; i < NUM_FRAME_DATA; ++i ) {
        VK_CHECK(vkCreateFence(vkContext.logicalDevice, &fenceInfo, nullptr, &commandBufferFences[i]));
    }

    CLogger::Debug("Created command buffers");
}

void CVulkanRendererImpl::CreateSemaphores()
{
    imageAvailableSemaphores.resize(NUM_FRAME_DATA);
    renderFinishedSemaphores.resize(NUM_FRAME_DATA);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (size_t i = 0; i < NUM_FRAME_DATA; i++)
    {
        VK_CHECK(vkCreateSemaphore(vkContext.logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]));
        VK_CHECK(vkCreateSemaphore(vkContext.logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]));
    }
}

void CVulkanRendererImpl::CleanupSwapChain()
{
    for (auto framebuffer : vkSwapChainFrameBuffers)
    {
        vkDestroyFramebuffer(vkContext.logicalDevice, framebuffer, nullptr);
    }

    vkDestroyPipeline(vkContext.logicalDevice, vkGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(vkContext.logicalDevice, vkPipelineLayout, nullptr);

    vkDestroyRenderPass(vkContext.logicalDevice, vkContext.renderPass, nullptr);

    for (auto imageView : vkSwapChainImageViews)
    {
        vkDestroyImageView(vkContext.logicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(vkContext.logicalDevice, vkSwapChain, nullptr);

//    for (size_t i = 0; i < vkSwapChainImages.size(); i++)
//    {
//        vkDestroyBuffer(vkContext.logicalDevice, uniformBuffers[i], nullptr);
//        vkFreeMemory(vkContext.logicalDevice, uniformBuffersMemory[i], nullptr);
//    }

    vkDestroyDescriptorPool(vkContext.logicalDevice, vkContext.descriptorPool, nullptr);
}






