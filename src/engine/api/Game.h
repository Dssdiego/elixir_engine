//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef VULKAN_ENGINE_GAME_H
#define VULKAN_ENGINE_GAME_H

#include <Tracy.hpp>
#include <string>
#include "../common/structs.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"
#include "../rendering/Renderer.h"
#include "../rendering/EngineRenderer.h"
#include "../gui/EditorInterface.h"
#include "../profiling/Logger.h"
#include "../scenes/SceneSystem.h"
#include "../sdks/GeforceNow.h"

class Game
{
public:
    void Init(EngineConfig* pConfig); // initializes everything in the engine
    void Run();  // main loop
    void Draw(); // draw loop
    void Cleanup(); // shuts down the engine

private:
    size_t frames = 0;
    int frameCount = 0;
    double previousTime = glfwGetTime();
};


#endif //VULKAN_ENGINE_GAME_H
