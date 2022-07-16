//
// Created by Diego S. Seabra on 08/03/22.
//

#include <Tracy.hpp>
#include "Game.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"
#include "../rendering/Renderer.h"
#include "../gui/EditorInterface.h"
#include "../profiling/Logger.h"
#include "../scenes/SceneSystem.h"
#include "../sdks/GeforceNow.h"


/*
 * Methods
 */
void Game::Init(EngineConfig* pConfig)
{
    // init engine systems
    Logger::Init();
    SceneSystem::Init();
    CWindow::Init(pConfig);
    AudioEngine::Init();
    Input::Init();
    Renderer::Init(GraphicsBackend::VULKAN);
    EditorInterface::Init();
//    CGeforceNow::Init();
}

void Game::Run()
{
    while(!CWindow::ShouldCloseWindow())
    {
        CWindow::Update();
        glfwPollEvents();

        Draw();

        AudioEngine::Update();

        FrameMark;
    }
}

void Game::Draw()
{
    // REVIEW: Does the editor draws BEFORE or AFTER the gamne?
    EditorInterface::Draw();
    Renderer::Draw();
}

void Game::Cleanup()
{
    // destroy engine systems
    SceneSystem::Shutdown();
    EditorInterface::Shutdown();
//    CGeforceNow::Shutdown();
    Renderer::Shutdown();
    Input::Shutdown();
    AudioEngine::Shutdown();
    CWindow::Shutdown();
}


