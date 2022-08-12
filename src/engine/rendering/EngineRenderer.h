//
// Created by Diego S. Seabra on 21/07/22.
//

#ifndef VULKAN_ENGINE_ENGINERENDERER_H
#define VULKAN_ENGINE_ENGINERENDERER_H

#include <cassert>
#include <vulkan/vulkan.h>
#include "VulkanSwapchain.h"
#include "VulkanDevice.h"

struct EngineRendererImpl
{
    VulkanSwapchain &swapChain;
    VulkanDevice &device;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIdx;
    int currentFrameIndex = 0;
    bool frameHasStarted = false;

    VkCommandBuffer GetCurrentCommandBuffer();

    void CreateCommandBuffers();
    void FreeCommandBuffers();

    VkCommandBuffer BeginFrame();
    void EndFrame();
};

class EngineRenderer
{
public:
    static void Init();
    static void Shutdown();

    static VkCommandBuffer BeginFrame();
    static void EndFrame();

    void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    void RecreateSwapChain();
};


#endif //VULKAN_ENGINE_ENGINERENDERER_H
