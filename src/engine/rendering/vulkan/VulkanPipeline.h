//
// Created by Diego S. Seabra on 14/08/22.
//

#ifndef VULKAN_ENGINE_VULKANPIPELINE_H
#define VULKAN_ENGINE_VULKANPIPELINE_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <vulkan/vulkan.h>
#include <string>

#include "VulkanDevice.h"
#include "../Shader.h"
#include "../shapes/Vertex.h"

struct PushConstantData
{
    glm::mat4 transform{1.f};
    alignas(32) glm::vec4 color;
};

struct PipelineConfig
{
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
    VkPipelineLayout pipelineLayout;

    // Methods
    void CreateGraphicsPipeline();

    // Helpers
    void CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule);
    void CreatePipelineLayout();
    void FillDefaultPipelineConfig();
    void Bind();
};

class VulkanPipeline
{
public:
    // REVIEW: Make it possible to pass a custom pipeline config at the creation of the pipeline?
    static void Init();
    static void Bind();
    static void Shutdown();

    static VkPipelineLayout GetPipelineLayout();
};


#endif //VULKAN_ENGINE_VULKANPIPELINE_H
