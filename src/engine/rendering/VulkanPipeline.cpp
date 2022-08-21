//
// Created by Diego S. Seabra on 14/08/22.
//

#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"

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
    CreatePipelineLayout();
    CreateGraphicsPipeline();
}

VulkanPipelineImpl::~VulkanPipelineImpl()
{
    Logger::Debug("Destroying shader modules");
    vkDestroyShaderModule(VulkanDevice::GetDevice(), vertShaderModule, nullptr);
    vkDestroyShaderModule(VulkanDevice::GetDevice(), fragShaderModule, nullptr);

    Logger::Debug("Destroying pipeline layout");
    vkDestroyPipelineLayout(VulkanDevice::GetDevice(), pipelineLayout, nullptr);

    Logger::Debug("Destroying graphics pipeline");
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

    // SECTION: Vertex Input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; // type of vertex data that will be passed to the vertex shader
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    // REVIEW: Vertex information should come from "model" information/class?
    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    // bindings: spacing between data and whether the data is per-vertex or per-instance
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

    // attribute descriptions: type of attributes passed to the vertex shader, which binding and at which offset
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // we create the rest of our graphics pipeline with the default configuration:
    //  -> Viewport from (0,0) to max of the screen
    //  -> Filled polygons (no wireframe, clockwise)
    //  -> No multisampling enabled
    //  -> Depth testing in VK_COMPARE_OP_LESS (lower depth = closer)
    //  -> "Normal/Tipical" color blending (combines the color returned from the fragment shader to the framebuffer color)
    FillDefaultPipelineConfig();

    // pipeline definition
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &pipelineConfig.inputAssemblyInfo;
    pipelineInfo.pViewportState = &pipelineConfig.viewportInfo;
    pipelineInfo.pRasterizationState = &pipelineConfig.rasterizationInfo;
    pipelineInfo.pMultisampleState = &pipelineConfig.multisampleInfo;
    pipelineInfo.pDepthStencilState = &pipelineConfig.depthStencilInfo;
    pipelineInfo.pColorBlendState = &pipelineConfig.colorBlendInfo;
    pipelineInfo.pDynamicState = &pipelineConfig.dynamicStateInfo;
    pipelineInfo.layout = pipelineConfig.pipelineLayout;
    pipelineInfo.renderPass = VulkanSwapchain::GetRenderPass();
    pipelineInfo.subpass = 0; // not using subpasses for now
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
    pipelineInfo.basePipelineIndex = -1; // optional

    VK_CHECK(vkCreateGraphicsPipelines(VulkanDevice::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));

    Logger::Debug("Graphics pipeline created");
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

    Logger::Debug("Created shader module");
}

void VulkanPipelineImpl::CreatePipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    VK_CHECK(vkCreatePipelineLayout(VulkanDevice::GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout));

    Logger::Debug("Created pipeline layout");
}

void VulkanPipelineImpl::FillDefaultPipelineConfig()
{
    // SECTION: Input Assembly
    pipelineConfig.inputAssemblyInfo =
    {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            nullptr,
            0,
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // because we want to draw a triangle for now
            false
    };

    // SECTION: Viewport
    pipelineConfig.viewportInfo =
    {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            nullptr,
            0,
            1, // REVIEW: More than one viewport makes a split screen game?
            nullptr, // viewport is defined by the dynamic states (below)
            1,
            nullptr // scissor is defined by the dynamic states (below)
    };

    // SECTION: Rasterization
    pipelineConfig.rasterizationInfo =
    {
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            nullptr,
            0,
            false,
            false, // setting this to VK_TRUE disabled any output to the framebuffer
            VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_BACK_BIT, // for now, we'll always cull the back face
            VK_FRONT_FACE_CLOCKWISE, // order for faces to be considered front-facing (in our case is counter clockwise because of MVP Y-flip in the shader)
            false,
            0.0f,
            0.0f,
            0.0f,
            1.0f
    };

    // SECTION: Multisampling
    pipelineConfig.multisampleInfo =
    {
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            nullptr,
            0,
            VK_SAMPLE_COUNT_1_BIT,
            false,
            1.0f,
            nullptr,
            false,
            false
    };

    // SECTION: Depth and Stencil Testing
    pipelineConfig.depthStencilInfo = {
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            nullptr,
            0,
            true,
            true,
            VK_COMPARE_OP_LESS,
            false,
            false,
            {},
            {},
            0.0f,
            1.0f
    };

    // SECTION: Color Blending
    pipelineConfig.colorBlendAttachment =
    {
            true,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_ADD,
            VK_BLEND_FACTOR_SRC_ALPHA,
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            VK_BLEND_OP_SUBTRACT,
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
    };

    pipelineConfig.colorBlendInfo = {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            nullptr,
            0,
            false,
            VK_LOGIC_OP_COPY,
            1,
            &pipelineConfig.colorBlendAttachment,
            {0.0f, 0.0f, 0.0f, 0.0f}
    };

    // SECTION: Dynamic States (Viewport and Scissor)
    pipelineConfig.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    pipelineConfig.dynamicStateInfo = {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            nullptr,
            0,
            static_cast<uint32_t>(pipelineConfig.dynamicStateEnables.size()),
            pipelineConfig.dynamicStateEnables.data()
    };

    // SECTION: Pipeline Layout
    pipelineConfig.pipelineLayout = pipelineLayout;
}
