//
// Created by Diego S. Seabra on 09/07/22.
//

#ifndef VULKAN_ENGINE_VULKANSWAPCHAIN_H
#define VULKAN_ENGINE_VULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

#include "../profiling/Logger.h"

class VulkanSwapchain
{
public:
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t imageCount;

private:
    void Init();
    void CreateSwapChain();
//    void CreateImageViews();
//    void CreateRenderPass();
//    void CreateDepthResources();
//    void CreateFramebuffers();
//    void CreateSyncObjects();
};


#endif //VULKAN_ENGINE_VULKANSWAPCHAIN_H
