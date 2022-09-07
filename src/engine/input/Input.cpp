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
        Camera::MoveRight();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_A))
        Camera::MoveLeft();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_W))
        Camera::MoveUp();

    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_S))
        Camera::MoveDown();
}
