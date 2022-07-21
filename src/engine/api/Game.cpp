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
//    Renderer::Init(GraphicsBackend::OPENGL);
//    EditorInterface::Init();
//    CGeforceNow::Init();
}

void Game::Run()
{
    Window::UpdateFPSInTitle(0.0f);

    while(!Window::ShouldCloseWindow())
    {
        double currentTime = Window::GetTime();
        double delta = currentTime - previousTime;
        frameCount++;

        // FPS calculation
        if (delta >= 2.0f) // we update the fps info every two seconds
        {
            int fps = int(frameCount / delta);
            Window::UpdateFPSInTitle(fps);

            frameCount = 0;
            previousTime = currentTime;
        }

        Window::Update();
        glfwPollEvents();

//        Draw();

        AudioEngine::Update();

        // increment the frame number
        frames++;
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
//    CGeforceNow::Shutdown();
    SceneSystem::Shutdown();
//    EditorInterface::Shutdown();
//    Renderer::Shutdown();
    Input::Shutdown();
    AudioEngine::Shutdown();
    Window::Shutdown();
}


