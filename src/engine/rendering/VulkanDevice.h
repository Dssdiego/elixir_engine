//
// Created by Diego S. Seabra on 09/07/22.
//

#ifndef VULKAN_ENGINE_VULKANDEVICE_H
#define VULKAN_ENGINE_VULKANDEVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <optional>
#include "Window.h"
#include "VulkanCommon.h"
#include "../profiling/Logger.h"
#include <cstring>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


struct VulkanDeviceImpl
{
    VulkanDeviceImpl();
    ~VulkanDeviceImpl();

    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDeviceAndQueues();
    void CreateCommandPool();

    // helper methods
    bool IsDeviceSuitable(VkPhysicalDevice device);
//    std::vector<const char *> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
//    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo, const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
//    void HasGflwRequiredInstanceExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

    // command buffer single time commands
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    // variables
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool;

    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    std::optional<uint32_t> graphicsFamilyIdx;
    std::optional<uint32_t> presentFamilyIdx;

    const std::vector<const char *> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

// REVIEW: Transform this class in a singleton?
class VulkanDevice
{
public:
    static void Init();
    static void Shutdown();

//    VulkanDevice();
//    ~VulkanDevice();

    // Not copyable or movable
    VulkanDevice(const VulkanDevice &) = delete;
    VulkanDevice &operator=(const VulkanDevice &) = delete;
    VulkanDevice(VulkanDevice &&) = delete;
    VulkanDevice &operator=(VulkanDevice &&) = delete;

    static VkInstance GetInstance();
    static VkCommandPool GetCommandPool();
    static VkDevice GetDevice();
    static VkPhysicalDevice GetPhysicalDevice();
    static VkSurfaceKHR GetSurface();
    static VkQueue GetGraphicsQueue();
    static VkQueue GetPresentQueue();
    static uint32_t GetGraphicsQueueFamilyIdx();
    static uint32_t GetPresentQueueFamilyIdx();

    static SwapChainSupportDetails GetSwapChainSupport();
//    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//    QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(physicalDevice); }
//    VkFormat FindSupportedFormat(
//            const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
//
//    // Buffer Helper Functions
//    void CreateBuffer(
//            VkDeviceSize size,
//            VkBufferUsageFlags usage,
//            VkMemoryPropertyFlags properties,
//            VkBuffer &buffer,
//            VkDeviceMemory &bufferMemory);
    static VkCommandBuffer BeginSingleTimeCommands();
    static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
//    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//    void CopyBufferToImage(
//            VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
//
//    void CreateImageWithInfo(
//            const VkImageCreateInfo &imageInfo,
//            VkMemoryPropertyFlags properties,
//            VkImage &image,
//            VkDeviceMemory &imageMemory);

    VkPhysicalDeviceProperties properties;
};


#endif //VULKAN_ENGINE_VULKANDEVICE_H
