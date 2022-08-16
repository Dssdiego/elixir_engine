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

void VulkanPipeline::Shutdown()
{
    delete mVulkanPipelineImpl;
}

//
// Implementation
//

VulkanPipelineImpl::VulkanPipelineImpl()
{
    CreateGraphicsPipeline();
}

VulkanPipelineImpl::~VulkanPipelineImpl()
{
    vkDestroyShaderModule(VulkanDevice::GetDevice(), vertShaderModule, nullptr);
    vkDestroyShaderModule(VulkanDevice::GetDevice(), fragShaderModule, nullptr);
    vkDestroyPipeline(VulkanDevice::GetDevice(), graphicsPipeline, nullptr);
}

void VulkanPipelineImpl::CreateGraphicsPipeline()
{
    // SECTION: Shaders
    auto vertShaderCode = Shader::ReadFile("assets/shaders/vert.spv");
    auto fragShaderCode = Shader::ReadFile("assets/shaders/frag.spv");

    CreateShaderModule(vertShaderCode, &vertShaderModule);
    CreateShaderModule(fragShaderCode, &fragShaderModule);

    // assigning vertex shader to the pipeline stage
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main"; // this allows us to combine multiple shaders in a single shader module (using multiple entry points)

    // assigning fragment shader to the pipeline stage
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    // TODO: Continue here --> Vertex Input and Assembly
}

//
// Helpers
//

void VulkanPipelineImpl::CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    VK_CHECK(vkCreateShaderModule(VulkanDevice::GetDevice(), &createInfo, nullptr, shaderModule));
}




