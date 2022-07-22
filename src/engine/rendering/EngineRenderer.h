//
// Created by Diego S. Seabra on 21/07/22.
//

#ifndef VULKAN_ENGINE_ENGINERENDERER_H
#define VULKAN_ENGINE_ENGINERENDERER_H

#include <vulkan/vulkan.h>
#include "VulkanSwapchain.h"
#include "VulkanDevice.h"

class EngineRenderer
{
public:
    EngineRenderer();
    ~EngineRenderer();

    VkCommandBuffer BeginFrame();
    void EndFrame();

    void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    VulkanSwapchain &swapChain;
    VulkanDevice &device;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIdx;
    int currentFrameIndex = 0;
    bool frameHasStarted = false;
};


#endif //VULKAN_ENGINE_ENGINERENDERER_H
