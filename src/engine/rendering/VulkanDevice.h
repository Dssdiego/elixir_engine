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

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class VulkanDevice
{
public:
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VulkanDevice();
    ~VulkanDevice();

    // Not copyable or movable
    VulkanDevice(const VulkanDevice &) = delete;
    VulkanDevice &operator=(const VulkanDevice &) = delete;
    VulkanDevice(VulkanDevice &&) = delete;
    VulkanDevice &operator=(VulkanDevice &&) = delete;

    VkCommandPool GetCommandPool() { return commandPool; }
    VkDevice GetDevice() { return device; }
    VkSurfaceKHR GetSurface() { return surface; }
    VkQueue GetGraphicsQueue() { return graphicsQueue; }
    VkQueue GetPresentQueue() { return presentQueue; }

//    SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(physicalDevice); }
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
//    VkCommandBuffer beginSingleTimeCommands();
//    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
//    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//    void CopyBufferToImage(
//            VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
//
//    void CreateImageWithInfo(
//            const VkImageCreateInfo &imageInfo,
//            VkMemoryPropertyFlags properties,
//            VkImage &image,
//            VkDeviceMemory &imageMemory);

    VkPhysicalDeviceProperties properties{};

private:
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
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo, const VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debugMessenger);
//    void HasGflwRequiredInstanceExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

    // variables
    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger{};
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool{};

    VkDevice device{};
    VkSurfaceKHR surface{};
    VkQueue graphicsQueue{};
    VkQueue presentQueue{};

    const std::vector<const char *> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};


#endif //VULKAN_ENGINE_VULKANDEVICE_H
