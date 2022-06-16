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

    const int NUM_FRAME_DATA = 2; // double buffering

    // Variables
    VkSurfaceKHR vkSurface;

    VkSwapchainKHR vkSwapChain;
    std::vector<VkImage> vkSwapChainImages;
    VkFormat vkSwapChainImageFormat;
    VkExtent2D vkSwapChainExtent;

    std::vector<VkImageView> vkSwapChainImageViews;
    std::vector<VkFramebuffer> vkSwapChainFrameBuffers;

    VkImage vkDepthImage;
    VkDeviceMemory vkDepthImageMemory;
    VkImageView vkDepthImageView;

    VkDescriptorSetLayout vkDescriptorSetLayout;

    VkPipeline vkGraphicsPipeline;
    VkPipelineLayout vkPipelineLayout;

    VkCommandPool vkCommandPool;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<VkFence> commandBufferFences;

    size_t currentFrame = 0;

    // Auxiliary Methods
    bool CheckValidationLayerSupport();
    bool IsDeviceSuitable(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                     VkDeviceMemory& imageMemory);
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    // command buffer single time commands
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    // Methods
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDeviceAndQueues();
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
    void CreateCommandBuffer();
    void CreateSemaphores();

    void CleanupSwapChain();

    void RecreateSwapChain();

    void DrawFrame();
//    void StartFrame();
//    void EndFrame();
};


class CVulkanRenderer
{
public:
    static void Init();
    static void Draw();
    static void Shutdown();

    // command buffer single time commands
    static VkCommandBuffer BeginSingleTimeCommands();
    static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
};


#endif //VULKAN_ENGINE_VULKANRENDERER_H
