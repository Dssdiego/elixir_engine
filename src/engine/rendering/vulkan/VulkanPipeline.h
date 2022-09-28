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
#include <sstream>
#include <string>

#include "VulkanSwapchain.h"
#include "VulkanPipelineBuilder.h"
#include "VulkanDevice.h"
#include "VulkanEngine.h"
#include "../Shader.h"
#include "../shapes/Vertex.h"

struct PushConstantData
{
    glm::mat4 transform{1.f};
    alignas(32) glm::vec4 color;
};

struct VulkanPipelineImpl
{
    VulkanPipelineImpl();
    ~VulkanPipelineImpl();

    // builder
    VulkanPipelineBuilder pipelineBuilder;

    // sets
    std::vector<PipelineSet> pipelineSets = {};
    uint32_t currentPipelineIdx = 0;

    // pipelines
    VkPipeline shapePipeline;
    VkPipelineLayout shapePipelineLayout;
    VkPipeline spritePipeline;
    VkPipelineLayout spritePipelineLayout;
    VkPipeline uiPipeline;
    VkPipelineLayout uiPipelineLayout;
    VkPipeline gridPipeline;
    VkPipelineLayout gridPipelineLayout;

    // Helpers
    void CreatePipelineSets();
    void CreatePipelineLayout(VkPipelineLayout &layout);
    void SwitchToPipeline(uint32_t index);
    PipelineSet GetCurrent();
    void Bind();
};

class VulkanPipeline
{
public:
    // REVIEW: Make it possible to pass a custom pipeline config at the creation of the pipeline?
    static void Init();
    static void Bind();
    static void Shutdown();

    static void SwitchToPipeline(uint32_t index);
    static PipelineSet GetCurrent();
};


#endif //VULKAN_ENGINE_VULKANPIPELINE_H
