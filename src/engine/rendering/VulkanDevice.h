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

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

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
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    QueueFamilyIndices FindPhysicalQueueFamilies();
    bool IsDeviceSuitable(VkPhysicalDevice device);
//    std::vector<const char *> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
//    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo, const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
//    void HasGflwRequiredInstanceExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

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

//    std::optional<uint32_t> graphicsFamilyIdx;
//    std::optional<uint32_t> presentFamilyIdx;

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
    static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//    QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(physicalDevice); }
//    VkFormat FindSupportedFormat(
//            const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
//
    // Helper Functions
    static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

    // Commands
    static VkCommandBuffer BeginSingleTimeCommands();
    static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    VkPhysicalDeviceProperties properties;
};


#endif //VULKAN_ENGINE_VULKANDEVICE_H
