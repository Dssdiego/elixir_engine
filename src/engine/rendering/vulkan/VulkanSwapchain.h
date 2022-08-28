//
// Created by Diego S. Seabra on 09/07/22.
//

#ifndef VULKAN_ENGINE_VULKANSWAPCHAIN_H
#define VULKAN_ENGINE_VULKANSWAPCHAIN_H

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <array>
#include <limits>

#include "VulkanDevice.h"
#include "VulkanImage.h"

#include "../../profiling/Logger.h"

struct VulkanSwapChainImpl
{
    VulkanSwapChainImpl();
    ~VulkanSwapChainImpl();

    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDepthResources();
    void CreateFrameBuffers();
    void CreateSyncObjects();

    VkResult AcquireNextImage(uint32_t *imageIndex);
    VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

    // helpers
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    uint32_t GetImageCount();
    VkRenderPass GetRenderPass();
    VkFramebuffer GetFrameBuffer(int index);
    VkExtent2D GetSwapChainExtent();

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

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    size_t currentFrame = 0; // maximum size of a theoretically possible object
    int MAX_FRAMES_IN_FLIGHT = 2;
};

class VulkanSwapchain
{
public:
    static void Init();
    static void Shutdown();
    static void Recreate();

    static uint32_t GetImageCount();
    static uint32_t GetNumberOfFramesInFlight();

    static VkResult AcquireNextImage(uint32_t *imageIndex);
    static VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

    static VkRenderPass GetRenderPass();
    static VkFramebuffer GetFrameBuffer(int index);
    static VkExtent2D GetSwapChainExtent();
};

#endif //VULKAN_ENGINE_VULKANSWAPCHAIN_H
