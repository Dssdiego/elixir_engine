//
// Created by Diego S. Seabra on 17/08/22.
//

#include "TestRenderSystem.h"
#include "VulkanPipeline.h"
#include "EngineRenderer.h"
#include "shapes/Triangle.h"

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
    // TODO: Continue here
//    mTestRenderSystemImpl->RenderGameObjects();
}

//
// Implementation
//

TestRenderSystemImpl::TestRenderSystemImpl()
{
    VulkanPipeline::Init();
}

TestRenderSystemImpl::~TestRenderSystemImpl()
{
    VulkanPipeline::Shutdown();
}

// REVIEW: Probably the render system knows too much about vulkan and pushing constants
//          Perhaps this is a responsibility for the EngineRenderer?
void TestRenderSystemImpl::RenderGameObjects()
{
    Triangle triangle = Triangle();
    triangle.Draw();

    // TODO: Go to each object in the array and render it
//    for (auto &obj : gameObjects)
//    {
//        auto commandBuffer = EngineRenderer::GetCurrentCommandBuffer();
//
//        SimplePushConstantData push{};
////        push.offset = obj.transform2d.translation;
////        push.color = obj.color;
////        push.transform = obj.transform2d.mat2();
//
//        vkCmdPushConstants(
//                commandBuffer,
//                nullptr,
//                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
//                0,
//                sizeof(SimplePushConstantData),
//                &push
//        );

//        obj.model.bind();
//        obj.model.draw();
//    }
}
