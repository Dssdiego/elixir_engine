//
// Created by Diego S. Seabra on 14/08/22.
//

#ifndef VULKAN_ENGINE_VULKANPIPELINE_H
#define VULKAN_ENGINE_VULKANPIPELINE_H

#include "VulkanDevice.h"
#include "Shader.h"
#include <vulkan/vulkan.h>
#include <string>

struct VulkanPipelineImpl
{
    VulkanPipelineImpl();
    ~VulkanPipelineImpl();

    VkPipeline graphicsPipeline;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    // Methods
    void CreateGraphicsPipeline();

    // Helpers
    void CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule);
};

class VulkanPipeline
{
    void Init();
    void Shutdown();
};


#endif //VULKAN_ENGINE_VULKANPIPELINE_H
