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

void VulkanPipeline::SwitchToPipeline(uint32_t index)
{
    mVulkanPipelineImpl->SwitchToPipeline(index);
}

PipelineSet VulkanPipeline::GetCurrent()
{
    return mVulkanPipelineImpl->GetCurrent();
}

//
// Implementation
//

VulkanPipelineImpl::VulkanPipelineImpl()
{
    CreatePipelineSets();
}

VulkanPipelineImpl::~VulkanPipelineImpl()
{
    // TODO: We need to wait for commands to complete before destroying the pipeline
    vkDeviceWaitIdle(VulkanDevice::GetDevice());

    pipelineBuilder.Cleanup();

    for (auto &pipelineSet : pipelineSets)
    {
        std::stringstream ss;
        ss << "Destroying pipeline '" << pipelineSet.name << "'...";
        Logger::Debug(ss.str());

        Logger::Debug("Destroying pipeline layout");
        vkDestroyPipelineLayout(VulkanDevice::GetDevice(), pipelineSet.layout, nullptr);

        Logger::Debug("Destroying graphics pipeline");
        vkDestroyPipeline(VulkanDevice::GetDevice(), pipelineSet.pipeline, nullptr);
    }
}

//
// Helpers
//

void VulkanPipelineImpl::CreatePipelineSets()
{
    PipelineSet shapePipelineSet
    {
            "shape",
            "assets/shaders/shape.vert.spv",
            "assets/shaders/shape.frag.spv",
            shapePipeline,
            shapePipelineLayout,
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VK_POLYGON_MODE_FILL,
            VK_TRUE
    };

    pipelineSets.push_back(shapePipelineSet);

    PipelineSet spritePipelineSet
    {
            "sprite",
            "assets/shaders/sprite.vert.spv",
            "assets/shaders/sprite.frag.spv",
            spritePipeline,
            spritePipelineLayout,
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VK_POLYGON_MODE_FILL,
            VK_TRUE
    };
    pipelineSets.push_back(spritePipelineSet);

    for (auto &pipelineSet : pipelineSets)
    {
        CreatePipelineLayout(pipelineSet.layout);
        pipelineBuilder.Build(pipelineSet);
    }

    // we always start with the first created pipeline
    currentPipelineIdx = 0;
}

// TODO: Make this method agnostic/dynamic
void VulkanPipelineImpl::CreatePipelineLayout(VkPipelineLayout &layout)
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PushConstantData);

    // TODO: Make it possible to pass any descriptor set layout we want in the pipeline
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{VulkanEngine::GetDescriptorSetLayout()};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    VK_CHECK(vkCreatePipelineLayout(VulkanDevice::GetDevice(), &pipelineLayoutInfo, nullptr, &layout));

    Logger::Debug("Created pipeline layout");
}

void VulkanPipelineImpl::Bind()
{
    vkCmdBindPipeline(
            VulkanEngine::GetCurrentCommandBuffer(),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            GetCurrent().pipeline
    );

    vkCmdBindDescriptorSets(
            VulkanEngine::GetCurrentCommandBuffer(),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            GetCurrent().layout,
            0,
            1,
            &VulkanEngine::GetDescriptorSets()[VulkanEngine::GetFrameIndex()], // REVIEW: My code is possibly wrong here!
            0,
            nullptr
            );
}

PipelineSet VulkanPipelineImpl::GetCurrent()
{
    return pipelineSets[currentPipelineIdx];
}

// TODO: Allow to swtich pipelines based on the pipeline name
void VulkanPipelineImpl::SwitchToPipeline(uint32_t index)
{
    currentPipelineIdx = index;
}
