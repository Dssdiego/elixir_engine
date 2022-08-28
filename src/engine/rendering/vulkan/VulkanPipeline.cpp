//
// Created by Diego S. Seabra on 14/08/22.
//

#include "VulkanPipeline.h"

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

// TODO: This should return the current pipeline layout (the pipeline layout of the current pipeline)
VkPipelineLayout VulkanPipeline::GetPipelineLayout()
{
    return mVulkanPipelineImpl->shapePipelineLayout;
}

//
// Implementation
//

VulkanPipelineImpl::VulkanPipelineImpl()
{
    CreateShapePipeline();
}

VulkanPipelineImpl::~VulkanPipelineImpl()
{
    // TODO: We need to wait for commands to complete before destroying the pipeline
    vkDeviceWaitIdle(VulkanDevice::GetDevice());

    pipelineBuilder.Cleanup();

    Logger::Debug("Destroying pipeline layout");
    vkDestroyPipelineLayout(VulkanDevice::GetDevice(), shapePipelineLayout, nullptr);

    Logger::Debug("Destroying graphics pipeline");
    vkDestroyPipeline(VulkanDevice::GetDevice(), shapePipeline, nullptr);
}

void VulkanPipelineImpl::CreateShapePipeline()
{
    CreatePipelineLayout();

    PipelineBuilderConfig config
            {
                    shapePipelineLayout,
                    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                    VK_POLYGON_MODE_FILL,
                    VK_TRUE
            };

    // TODO: Clean up pipeline builder stuff after we are done with it
    shapePipeline = pipelineBuilder.Build(config);
}

void VulkanPipelineImpl::CreateSpritePipeline()
{

}

//
// Helpers
//

// TODO: Make this method agnostic/dynamic
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

    VK_CHECK(vkCreatePipelineLayout(VulkanDevice::GetDevice(), &pipelineLayoutInfo, nullptr, &shapePipelineLayout));

    Logger::Debug("Created pipeline layout");
}

void VulkanPipelineImpl::Bind()
{
    vkCmdBindPipeline(EngineRenderer::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, shapePipeline);
}
