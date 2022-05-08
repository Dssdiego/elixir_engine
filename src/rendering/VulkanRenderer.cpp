//
// Created by Diego S. Seabra on 14/03/22.
//

#include "VulkanRenderer.h"

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

}

void CVulkanRendererImpl::PickPhysicalDevice()
{

}

void CVulkanRendererImpl::CreateLogicalDevice()
{

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


