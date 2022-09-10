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
#include "descriptors/VulkanDescriptorPool.h"
#include "descriptors/VulkanDescriptorSetLayout.h"

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

    std::unique_ptr<VulkanDescriptorPool> descriptorPool;
    std::unique_ptr<VulkanDescriptorSetLayout> descriptorSetLayout;
    std::vector<std::unique_ptr<VulkanBuffer>> uniformBuffers;

    VkCommandBuffer GetCurrentCommandBuffer();
    int GetFrameIndex();

    void CreateUniformBuffers();
    void UpdateUniformBuffer(UniformBufferObject *ubo);

    void CreateCommandBuffers();
    void FreeCommandBuffers();

    void CreateDescriptors();

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

    static VkDescriptorSetLayout GetDescriptorSetLayout();

    static VkCommandBuffer GetCurrentCommandBuffer();
    static int GetFrameIndex();
    static VkCommandBuffer BeginFrame();
    static void EndFrame();

    static void UpdateUniformBuffer(UniformBufferObject *ubo);
};


#endif //VULKAN_ENGINE_VULKANENGINE_H