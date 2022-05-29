//
// Created by Diego S. Seabra on 28/05/22.
//

#ifndef VULKAN_ENGINE_VULKANCONTEXT_H
#define VULKAN_ENGINE_VULKANCONTEXT_H

#include <vulkan/vulkan.h>

struct SVulkanContext {
    // instance
    VkInstance instance;

    // devices
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;

    // queues
    int graphicsFamilyIdx;
    int presentFamilyIdx;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    // format
    VkFormat depthFormat;

    // render pass
    VkRenderPass renderPass;

    // descriptor pool
    VkDescriptorPool descriptorPool;

    // swap chain
    uint32_t swapChainImageCount;
};

extern SVulkanContext vkContext;

#endif //VULKAN_ENGINE_VULKANCONTEXT_H
