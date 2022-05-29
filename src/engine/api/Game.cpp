//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Game.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"
#include "../rendering/Renderer.h"
#include "../gui/EditorInterface.h"
#include "../profiling/Logger.h"

/*
 * Engine Constants
 */
uint32_t vMajor = 0;
uint32_t vMinor = 1;
uint32_t vPatch = 0;

std::string engineName = "Elixir Engine";
std::string engineVersion = "v0.1.0-alpha";

/*
 * Methods
 */
void CGame::Init(SEngineConfig* pConfig)
{
    // init engine systems
    CLogger::Init();
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
    // REVIEW: Does the editor draws BEFORE or AFTER the gamne?
    CEditorInterface::Draw();
    CRenderer::Draw();
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


