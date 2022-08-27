//
// Created by Diego S. Seabra on 14/08/22.
//

#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"
#include "../EngineRenderer.h"
#include "VulkanPipelineBuilder.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
VulkanPipelineImpl* mVulkanPipelineImpl = nullptr;

//
// Initialization/Destruction
//

void VulkanPipeline::Init()
{
    mVulkanPipelineImpl = new VulkanPipelineImpl;
}

void VulkanPipeline::Bind()
{
    mVulkanPipelineImpl->Bind();
}

void VulkanPipeline::Shutdown()
{
    delete mVulkanPipelineImpl;
}

VkPipelineLayout VulkanPipeline::GetPipelineLayout()
{
    return mVulkanPipelineImpl->graphicsPipelineLayout;
}

//
// Implementation
//

VulkanPipelineImpl::VulkanPipelineImpl()
{
    CreatePipelineLayout();

    PipelineBuilderConfig config
    {
        graphicsPipelineLayout,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        VK_POLYGON_MODE_FILL,
        VK_TRUE
    };

    // TODO: Clean up pipeline builder stuff after we are done with it
    VulkanPipelineBuilder pipelineBuilder;
    graphicsPipeline = pipelineBuilder.Build(config);
}

VulkanPipelineImpl::~VulkanPipelineImpl()
{
    // TODO: We need to wait for commands to complete before destroying the pipeline
    vkDeviceWaitIdle(VulkanDevice::GetDevice());


    Logger::Debug("Destroying shader modules");
    vkDestroyShaderModule(VulkanDevice::GetDevice(), vertShaderModule, nullptr);
    vkDestroyShaderModule(VulkanDevice::GetDevice(), fragShaderModule, nullptr);

    Logger::Debug("Destroying pipeline layout");
    vkDestroyPipelineLayout(VulkanDevice::GetDevice(), graphicsPipelineLayout, nullptr);

    Logger::Debug("Destroying graphics pipeline");
    vkDestroyPipeline(VulkanDevice::GetDevice(), graphicsPipeline, nullptr);
}

//
// Helpers
//

void VulkanPipelineImpl::CreatePipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    VK_CHECK(vkCreatePipelineLayout(VulkanDevice::GetDevice(), &pipelineLayoutInfo, nullptr, &graphicsPipelineLayout));

    Logger::Debug("Created pipeline layout");
}

void VulkanPipelineImpl::Bind()
{
    vkCmdBindPipeline(EngineRenderer::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}
