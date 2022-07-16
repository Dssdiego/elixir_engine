//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"
#include "../profiling/Profiler.h"
#include "../profiling/Logger.h"

InputImpl* mInputImpl = nullptr;

InputImpl::InputImpl()
{
    // TODO: Implement
}

InputImpl::~InputImpl()
{
    // TODO: Implement
}

void Input::Init()
{
    Logger::Info("Initializing input system");
    PROFILE_FUNCTION();
    mInputImpl = new InputImpl;
}

void Input::Shutdown()
{
    Logger::Info("Shutting down input system");
    PROFILE_FUNCTION();
    delete mInputImpl;
}
