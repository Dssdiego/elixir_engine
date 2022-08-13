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
    VulkanSwapchain swapChain{};
    VulkanDevice device{};
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIdx{0};
    int currentFrameIndex{0};
    bool frameHasStarted{false};

    VkCommandBuffer GetCurrentCommandBuffer();

    void CreateCommandBuffers();
    void FreeCommandBuffers();

    VkCommandBuffer BeginFrame();
    void BeginSwapChainRenderPass();
    void EndSwapChainRenderPass();
    void EndFrame();
};

class EngineRenderer
{
public:
    static void Init();
    static void Shutdown();

    static VkCommandBuffer BeginFrame();
    static void EndFrame();
};


#endif //VULKAN_ENGINE_ENGINERENDERER_H
