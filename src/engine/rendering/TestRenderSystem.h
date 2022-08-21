//
// Created by Diego S. Seabra on 17/08/22.
//

#ifndef VULKAN_ENGINE_TESTRENDERSYSTEM_H
#define VULKAN_ENGINE_TESTRENDERSYSTEM_H

#include <iostream>
#include <vector>
#include "../common/GameObject.h"

struct TestRenderSystemImpl
{
    TestRenderSystemImpl();
    ~TestRenderSystemImpl();

    std::vector<GameObject> gameObjects;

    void RenderGameObjects();
};

class TestRenderSystem
{
public:
    static void Init();
    static void Shutdown();

    static void RenderGameObjects();
};


#endif //VULKAN_ENGINE_TESTRENDERSYSTEM_H
