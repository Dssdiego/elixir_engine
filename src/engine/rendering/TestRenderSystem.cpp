//
// Created by Diego S. Seabra on 17/08/22.
//

#include "TestRenderSystem.h"
#include "VulkanPipeline.h"

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

std::vector<GameObject>* TestRenderSystem::GetGameObjects()
{
    return mTestRenderSystemImpl->GetGameObjects();
}

//
// Implementation
//

TestRenderSystemImpl::TestRenderSystemImpl()
{
    VulkanPipeline::Init();

    // load triangle test game object
    auto triangle = GameObject::Create();
    triangle.id = 0;
    triangle.name = "Triangle";
    triangle.color[0] = 1.f;
    triangle.color[1] = 0.f;
    triangle.color[2] = 0.f;
    triangle.color[3] = 1.f;
    triangle.shape = Shape(ShapeType::Triangle);
    triangle.transform.position[0] = 0.f;
    triangle.transform.position[1] = 0.f;
    triangle.transform.position[2] = 0.f;
    triangle.transform.rotation = 0.0f;
    triangle.transform.scale[0] = 1.0f;
    triangle.transform.scale[1] = 1.0f;
    triangle.transform.scale[2] = 1.0f;
    // FIXME: Make sure we can use glm::vec2 and glm::vec3 with ImGui without converting the original field type!

    // load quad test game object
    auto quad = GameObject::Create();
    quad.id = 1;
    quad.name = "Quad";
    quad.color[0] = 1.f;
    quad.color[1] = 1.f;
    quad.color[2] = 0.f;
    quad.color[3] = 1.f;
    quad.shape = Shape(ShapeType::Quad);
    quad.transform.position[0] = 0.35f;
    quad.transform.position[1] = -0.2f;
    quad.transform.position[2] = 0.f;
    quad.transform.rotation = 0.f;
    quad.transform.scale[0] = 0.8f;
    quad.transform.scale[1] = 0.8f;
    quad.transform.scale[2] = 0.8f;

    // load circle test game object
//    auto circle = GameObject::Create();
//    circle.id = 2;
//    circle.name = "Circle";
//    circle.color[0] = 0.f;
//    circle.color[1] = 0.f;
//    circle.color[2] = 1.f;
//    circle.color[3] = 1.f;
//    circle.shape = Shape(ShapeType::Circle);
//    circle.transform.position[0] = -0.35f;
//    circle.transform.position[1] = -0.2f;
//    circle.transform.position[2] = 0.0f;
//    circle.transform.rotation = 0.f;
//    circle.transform.scale[0] = 0.3f;
//    circle.transform.scale[1] = 0.35f;
//    circle.transform.scale[2] = 0.35f;

    auto sprite = Sprite();
    sprite.id = 3;
    sprite.name = "A simple sprite";

    gameObjects.push_back(sprite);
//    gameObjects.push_back(circle);
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
//        push.offset = glm::vec2(obj.transform.position[0], obj.transform.position[1]);
        push.color = glm::vec4(obj.color[0], obj.color[1], obj.color[2], obj.color[3]);
        push.transform = obj.transform.mat4();

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

std::vector<GameObject>* TestRenderSystemImpl::GetGameObjects()
{
    return &gameObjects;
}
