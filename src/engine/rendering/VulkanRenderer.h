//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef VULKAN_ENGINE_VULKANRENDERER_H
#define VULKAN_ENGINE_VULKANRENDERER_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <optional>
#include <set>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct CVulkanRendererImpl
{
    // Constructor/Destructor
    CVulkanRendererImpl();
    ~CVulkanRendererImpl();

    // Constants
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // Variables
    VkInstance vkInstance;
    VkSurfaceKHR vkSurface;

    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE; // physical device
    VkDevice vkLogicalDevice; // logical device

    VkQueue vkGraphicsQueue;
    VkQueue vkPresentQueue;

    VkSwapchainKHR vkSwapChain;
    std::vector<VkImage> vkSwapChainImages;
    uint32_t swapChainImageCount;
    VkFormat vkSwapChainImageFormat;
    VkExtent2D vkSwapChainExtent;

    std::vector<VkImageView> vkSwapChainImageViews;

    VkRenderPass vkRenderPass;

    VkDescriptorPool vkDescriptorPool;

    // Auxiliary Methods
    bool CheckValidationLayerSupport();
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Methods
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateCommandPool();
    void CreateDepthResources();
    void CreateFramebuffers();
    void CreateTextureImage();
    void CreateTextureSampler();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void CreateCommandBuffers();
    void CreateSyncObjects();

    void CleanupSwapChain();
};


class CVulkanRenderer
{
public:
    static void Init();
    static void Draw();
    static void Shutdown();

    // External
    static VkInstance GetInstance();
    static VkPhysicalDevice GetPhysicalDevice();
    static VkDevice GetLogicalDevice();
    static VkQueue GetGraphicsQueue();
    static VkRenderPass GetRenderPass();
    static VkDescriptorPool GetDescriptorPool();
    static uint32_t GetSwapChainImageCount();
};


#endif //VULKAN_ENGINE_VULKANRENDERER_H
