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


/*
 * Methods
 */
void CGame::Init(SEngineConfig* pConfig)
{
    // init engine systems
    CLogger::Init();
    CSceneSystem::Init();
    CWindow::Init(pConfig);
    CAudioEngine::Init();
    CInput::Init();
    CRenderer::Init(SBackend::VULKAN);
    CEditorInterface::Init();
}

void CGame::Run()
{
    while(!CWindow::ShouldCloseWindow())
    {
        CWindow::Update();
        glfwPollEvents();

        Draw();

        CAudioEngine::Update();

        FrameMark;
    }
}

void CGame::Draw()
{
    // REVIEW: Does the editor draws BEFORE or AFTER the gamne?
    CEditorInterface::Draw();
    CRenderer::Draw();
}

void CGame::Cleanup()
{
    // destroy engine systems
    CSceneSystem::Shutdown();
    CEditorInterface::Shutdown();
    CRenderer::Shutdown();
    CInput::Shutdown();
    CAudioEngine::Shutdown();
    CWindow::Shutdown();
}


