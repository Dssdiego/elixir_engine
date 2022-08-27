//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"
#include "../profiling/Profiler.h"
#include "../profiling/Logger.h"

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
}
