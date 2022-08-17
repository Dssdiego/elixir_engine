//
// Created by Diego S. Seabra on 14/08/22.
//

#ifndef VULKAN_ENGINE_VULKANPIPELINE_H
#define VULKAN_ENGINE_VULKANPIPELINE_H

#include <vulkan/vulkan.h>
#include <string>

#include "VulkanDevice.h"
#include "Shader.h"
#include "Vertex.h"

struct PipelineConfig {
    // REVIEW: Delete any operators?
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

struct VulkanPipelineImpl
{
    VulkanPipelineImpl();
    ~VulkanPipelineImpl();

    VkPipeline graphicsPipeline;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    PipelineConfig pipelineConfig;

    // Methods
    void CreateGraphicsPipeline();

    // Helpers
    void CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule);
    void FillDefaultPipelineConfig();
};

class VulkanPipeline
{
    // TODO: Make it possible to pass a custom pipeline config at the creation of the pipeline
    void Init();
    void Shutdown();
};


#endif //VULKAN_ENGINE_VULKANPIPELINE_H
