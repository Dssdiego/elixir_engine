//
// Created by Diego S. Seabra on 09/07/22.
//

#ifndef VULKAN_ENGINE_VULKANSWAPCHAIN_H
#define VULKAN_ENGINE_VULKANSWAPCHAIN_H

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <array>
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

#include "../profiling/Logger.h"

struct VulkanSwapChainImpl
{
    VulkanSwapChainImpl();
    ~VulkanSwapChainImpl();

    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDepthResources();
    void CreateFramebuffers();
    void CreateSyncObjects();

    // helpers
    VkImageView VulkanSwapChainImpl::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                     VkDeviceMemory& imageMemory);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    // variables
    VkFormat swapChainImageFormat;
    VkFormat swapChainDepthFormat;

    VkExtent2D swapChainExtent;

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    VkSwapchainKHR swapChain;
    VkRenderPass renderPass;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkFramebuffer> swapChainFrameBuffers;
};

class VulkanSwapchain
{
public:
    static void Init();
    static void Shutdown();

    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t imageCount;

    // TODO: Implement
    static uint32_t GetImageCount() { return 0; }
};


#endif //VULKAN_ENGINE_VULKANSWAPCHAIN_H
