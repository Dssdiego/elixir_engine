//
// Created by Diego S. Seabra on 08/03/22.
//

#include "GameEngine.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"

void CGameEngine::Init(SEngineConfig* pConfig)
{
    // init engine systems
    CWindow::Init(pConfig);
    CAudioEngine::Init();
    CInput::Init();
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

}

void CGameEngine::Cleanup()
{
    // destroy engine systems
    CAudioEngine::Shutdown();
    CInput::Shutdown();
    CWindow::Shutdown();
}


