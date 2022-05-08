//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef VULKAN_ENGINE_VULKANRENDERER_H
#define VULKAN_ENGINE_VULKANRENDERER_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

struct CVulkanRendererImpl
{
    // Constructor/Destructor
    CVulkanRendererImpl();
    ~CVulkanRendererImpl();

    // Constants
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    // Variables
    VkInstance vkInstance;

    // Auxiliary Methods
    bool CheckValidationLayerSupport();

    // Methods
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateCommandPool();
    void CreateDepthResources();
    void CreateFramebuffers();
    void CreateTextureImage();
    void CreateImageView();
    void CreateTextureSampler();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void CreateCommandBuffers();
    void CreateSyncObjects();

};


class CVulkanRenderer
{
public:
    static void Init();
    static void Draw();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_VULKANRENDERER_H
