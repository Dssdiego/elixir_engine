//
// Created by Diego S. Seabra on 08/03/22.
//

#include "GameEngine.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"
#include "../rendering/Renderer.h"

void CGameEngine::Init(SEngineConfig* pConfig)
{
    // init engine systems
    CWindow::Init(pConfig);
    CAudioEngine::Init();
    CInput::Init();
    CRenderer::Init(SBackend::VULKAN);
}

void CGameEngine::Run()
{
    while(!CWindow::ShouldCloseWindow())
    {
        CWindow::Update();
        glfwPollEvents();

        Draw();

        CAudioEngine::Update();
    }
}

void CGameEngine::Draw()
{
    CRenderer::Draw();
}

void CGameEngine::Cleanup()
{
    // destroy engine systems
    CRenderer::Shutdown();
    CInput::Shutdown();
    CAudioEngine::Shutdown();
    CWindow::Shutdown();
}


