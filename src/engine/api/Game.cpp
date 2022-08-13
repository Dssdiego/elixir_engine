//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Game.h"

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
    EngineRenderer::Init();
//    Renderer::Init(GraphicsBackend::VULKAN);
//    EditorInterface::Init();
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

        // FPS calculation
        if (delta >= 2.0f) // we update the fps info every two seconds
        {
            int fps = int(frameCount / delta);
            Window::UpdateFPSInTitle(fps, frames);

            frameCount = 0;
            previousTime = currentTime;
        }

        Window::Update();
        glfwPollEvents();

        auto commandBuffer = EngineRenderer::BeginFrame();

//        RenderSystem::RenderGameObjects(commandBuffer, gameObjects);

        EngineRenderer::EndFrame();

//        Draw();

        AudioEngine::Update();

        // increment the frame number
        // REVIEW: Calculate frames in the Swapchain?
        frames++;
        FrameMark;
    }
}

void Game::Draw()
{
    // REVIEW: Does the editor draws BEFORE or AFTER the gamne?
//    EditorInterface::Draw();
//    Renderer::Draw();
}

void Game::Cleanup()
{
    // destroy engine systems
//    CGeforceNow::Shutdown();
    SceneSystem::Shutdown();
    EngineRenderer::Shutdown();
//    EditorInterface::Shutdown();
//    Renderer::Shutdown();
    Input::Shutdown();
    AudioEngine::Shutdown();
    Window::Shutdown();
}


