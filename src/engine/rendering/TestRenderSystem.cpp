//
// Created by Diego S. Seabra on 17/08/22.
//

#include "TestRenderSystem.h"
#include "VulkanPipeline.h"
#include "shapes/Triangle.h"
#include "shapes/Quad.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
TestRenderSystemImpl* mTestRenderSystemImpl = nullptr;

//
// Initialization/Destruction
//

void TestRenderSystem::Init()
{
    mTestRenderSystemImpl = new TestRenderSystemImpl;
}

void TestRenderSystem::Shutdown()
{
    delete mTestRenderSystemImpl;
}

void TestRenderSystem::RenderGameObjects()
{
    mTestRenderSystemImpl->RenderGameObjects();
}

//
// Implementation
//

TestRenderSystemImpl::TestRenderSystemImpl()
{
    VulkanPipeline::Init();

    // load triangle test game object
    auto triangle = GameObject::Create();
    triangle.color = {1.0f, 0.f, 0.f};
    triangle.shape = Triangle();
    triangle.transform.position = {0.f, 0.f};
    triangle.transform.rotation = 0.0f;
    triangle.transform.scale = {1.0f, 1.0f};

    // load quad test game object
    auto quad = GameObject::Create();
    quad.color = {0.f, 0.f, 1.f};
    quad.shape = Quad();
    quad.transform.position = {0.35f, -0.2f};
    quad.transform.rotation = 0.f;
    quad.transform.scale = {0.8f, 0.8f};

    // load textured quad test game object

    gameObjects.push_back(triangle);
    gameObjects.push_back(quad);
}

TestRenderSystemImpl::~TestRenderSystemImpl()
{
    for (auto &obj : gameObjects)
    {
        obj.shape.Destroy();
    }

    VulkanPipeline::Shutdown();
}

void TestRenderSystemImpl::RenderGameObjects()
{
    VulkanPipeline::Bind();

    for (auto &obj : gameObjects)
    {
        auto commandBuffer = EngineRenderer::GetCurrentCommandBuffer();

        PushConstantData push{};
        push.offset = obj.transform.position;
        push.color = obj.color;
        push.transform = obj.transform.mat2();

        vkCmdPushConstants(
                commandBuffer,
                VulkanPipeline::GetPipelineLayout(),
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(PushConstantData),
                &push
        );

        obj.shape.Draw();
    }
}
