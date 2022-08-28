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

#include "VulkanSwapchain.h"
#include "VulkanPipelineBuilder.h"
#include "VulkanDevice.h"
#include "../EngineRenderer.h"
#include "../Shader.h"
#include "../shapes/Vertex.h"

// The combination of a pipeline and it's layout
//struct PipelineSet
//{
//    VkPipeline pipeline;
//    VkPipelineLayout layout;
//};

//enum BoundPipeline
//{
//    SHAPE,
//    SPRITE
//};

struct PushConstantData
{
    glm::mat4 transform{1.f};
    alignas(32) glm::vec4 color;
};

struct VulkanPipelineImpl
{
    VulkanPipelineImpl();
    ~VulkanPipelineImpl();

//    BoundPipeline currentPipeline = BoundPipeline::SHAPE;

    // Shape Pipeline
    VkPipeline shapePipeline;
    VkPipelineLayout shapePipelineLayout;

    // Sprite Pipeline
    VkPipeline spritePipeline;
    VkPipelineLayout spritePipelineLayout;

    // Pipeline builder
    VulkanPipelineBuilder pipelineBuilder;

    // Helpers
    void CreateShapePipeline();
    void CreateSpritePipeline();
    void CreatePipelineLayout();
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
