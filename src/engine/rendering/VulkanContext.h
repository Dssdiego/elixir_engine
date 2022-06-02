//
// Created by Diego S. Seabra on 28/05/22.
//

#ifndef VULKAN_ENGINE_VULKANCONTEXT_H
#define VULKAN_ENGINE_VULKANCONTEXT_H

#include <vulkan/vulkan.h>
#include <vector>

struct SVulkanContext {
    // instance
    VkInstance instance;

    // devices
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;

    // queues and indices
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    uint32_t graphicsFamilyIdx;
    uint32_t presentFamilyIdx;

    // format
    VkFormat depthFormat;

    // render pass
    VkRenderPass renderPass;

    // descriptor pool
    VkDescriptorPool descriptorPool;

    // swap chain
    uint32_t swapChainImageCount;

    // command buffer
    std::vector<VkCommandBuffer> commandBuffers;
};

extern SVulkanContext vkContext;

#endif //VULKAN_ENGINE_VULKANCONTEXT_H
