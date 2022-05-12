//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Game.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"
#include "../rendering/Renderer.h"
#include "../gui/EditorInterface.h"

void CGame::Init(SEngineConfig* pConfig)
{
    // init engine systems
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
    }
}

void CGame::Draw()
{
    CRenderer::Draw();
    CEditorInterface::Draw();
}

void CGame::Cleanup()
{
    // destroy engine systems
    CEditorInterface::Shutdown();
    CRenderer::Shutdown();
    CInput::Shutdown();
    CAudioEngine::Shutdown();
    CWindow::Shutdown();
}


