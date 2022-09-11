//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"
#include "../profiling/Profiler.h"
#include "../profiling/Logger.h"
#include "../rendering/Window.h"
#include "../rendering/Camera.h"

void Input::Init()
{
    Logger::Info("Initializing input system");
}

void Input::Shutdown()
{
    Logger::Info("Shutting down input system");
}

void Input::Update()
{
    glfwPollEvents();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_D))
        if (Camera::HasControl())
            Camera::MoveRight();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_A))
        if (Camera::HasControl())
            Camera::MoveLeft();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_W))
        if (Camera::HasControl())
            Camera::MoveUp();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_S))
        if (Camera::HasControl())
            Camera::MoveDown();
}
