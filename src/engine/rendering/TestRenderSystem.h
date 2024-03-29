//
// Created by Diego S. Seabra on 17/08/22.
//

#ifndef VULKAN_ENGINE_TESTRENDERSYSTEM_H
#define VULKAN_ENGINE_TESTRENDERSYSTEM_H

#include <iostream>
#include <vector>
#include "../common/GameObject.h"
#include "../rendering/Sprite.h"

struct TestRenderSystemImpl
{
    TestRenderSystemImpl();
    ~TestRenderSystemImpl();

//    std::vector<GameObject> gameObjects;

    void RenderGameObjects(std::vector<GameObject> *gameObjects);
//    std::vector<GameObject>* GetGameObjects();

    size_t drawCalls = 0;
};

class TestRenderSystem
{
public:
    static void Init();
    static void Shutdown();

    static void RenderGameObjects(std::vector<GameObject> *gameObjects);
//    static std::vector<GameObject>* GetGameObjects();

    // REVIEW: This should be here? How we can count all draw calls without binding it to the render systems?
    static size_t GetDrawCallCount();
};


#endif //VULKAN_ENGINE_TESTRENDERSYSTEM_H
