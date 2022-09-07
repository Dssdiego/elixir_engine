//
// Created by Diego S. Seabra on 21/07/22.
//

#ifndef VULKAN_ENGINE_VULKANENGINE_H
#define VULKAN_ENGINE_VULKANENGINE_H

#include <cassert>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "VulkanSwapchain.h"
#include "VulkanDevice.h"
#include "../../common/Color.h"
#include "VulkanBuffer.h"

struct UniformBufferObject
{
//    glm::mat4 projectView{1.f};
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
};

struct EngineRendererImpl
{
    EngineRendererImpl();
    ~EngineRendererImpl();

    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIdx{0};
    int currentFrameIndex{0};
    bool frameHasStarted{false};

    // TODO: Make use of multiple uniform buffers
    std::unique_ptr<VulkanBuffer> globalUboBuffer;

    VkCommandBuffer GetCurrentCommandBuffer();
    int GetFrameIndex();

    void CreateUniformBuffer();
    void UpdateUniformBuffer(UniformBufferObject *ubo);
    void CreateCommandBuffers();
    void FreeCommandBuffers();

    VkCommandBuffer BeginFrame();
    void BeginSwapChainRenderPass();
    void EndSwapChainRenderPass();
    void EndFrame();
};

class VulkanEngine
{
public:
    static void Init();
    static void Shutdown();

    static VkCommandBuffer GetCurrentCommandBuffer();
    static int GetFrameIndex();
    static VkCommandBuffer BeginFrame();
    static void EndFrame();

    static void UpdateUniformBuffer(UniformBufferObject *ubo);
};


#endif //VULKAN_ENGINE_VULKANENGINE_H
