//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Game.h"
#include "../rendering/ImGuiRenderer.h"
#include "../rendering/TestRenderSystem.h"
#include "../rendering/text/Font.h"
#include "../core/Time.h"

/*
 * Methods
 */
void Game::Init(EngineConfig* pConfig)
{
    // init engine systems
    Logger::Init();
    SceneSystem::Init();
    Window::Init(pConfig);
    AudioEngine::Init();
    Input::Init();
    VulkanEngine::Init();
    ImGuiRenderer::Init();
    TestRenderSystem::Init();
//    Font::LoadFont();
//    Renderer::Init(GraphicsBackend::VULKAN);
//    CGeforceNow::Init();
}

void Game::Run()
{
    Window::UpdateFPSInTitle(0.0f, 0);

    while(!Window::ShouldCloseWindow())
    {
        // REVIEW: Probably won't need this because the renderer now has BeginFrame() and EndFrame() which could give us the FPS information
        //         Put this in the EngineRenderer class?
        double currentTime = Window::GetTime();
        double delta = currentTime - previousTime;
        frameCount++;

        // REVIEW: Calculate delta inside a "Time" class?
        Time::SetDelta(delta);

        // FPS calculation
        if (delta >= 2.0f) // we update the fps info every two seconds
        {
            int fps = int(frameCount / delta);
            Window::UpdateFPSInTitle(fps, frames);

            frameCount = 0;
            previousTime = currentTime;
        }

        Window::Update();
        Input::Update();

        Draw();

        AudioEngine::Update();

        // increment the frame number
        // REVIEW: Calculate frames in the Swapchain?
        frames++;
        FrameMark;
    }
}

void Game::Draw()
{
    VulkanEngine::BeginFrame();
    ImGuiRenderer::NewFrame();

    TestRenderSystem::RenderGameObjects();

    ImGuiRenderer::Render();
    VulkanEngine::EndFrame();
}

void Game::Cleanup()
{
    // destroy engine systems
//    CGeforceNow::Shutdown();
    TestRenderSystem::Shutdown();
    ImGuiRenderer::Shutdown();
    SceneSystem::Shutdown();
    VulkanEngine::Shutdown();
//    Renderer::Shutdown();
    Input::Shutdown();
    AudioEngine::Shutdown();
    Window::Shutdown();
}


