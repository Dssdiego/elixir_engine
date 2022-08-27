//
// Created by Diego S. Seabra on 27/08/22.
//

#ifndef VULKAN_ENGINE_VULKANPIPELINEBUILDER_H
#define VULKAN_ENGINE_VULKANPIPELINEBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>

struct PipelineBuilderConfig
{
//    std::string vertexShaderFilePath;
//    std::string fragmentShaderFilePath;
    VkPipelineLayout pipelineLayout;
    VkPrimitiveTopology topology;
    VkPolygonMode polygonMode;
    VkBool32 enableBlending;
};

class VulkanPipelineBuilder
{
public:
//    VkPipelineLayout pipelineLayout = nullptr;

    VkPipeline Build(PipelineBuilderConfig pipelineBuilderConfig);
//    VkPipeline Build();
    void Cleanup();

private:
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    // Helpers
    void CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule);
};


#endif //VULKAN_ENGINE_VULKANPIPELINEBUILDER_H
