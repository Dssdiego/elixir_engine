//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"
#include "../profiling/Profiler.h"
#include "../profiling/Logger.h"

CInputImpl* mImplementation = nullptr;

CInputImpl::CInputImpl()
{
    // TODO: Implement
}

CInputImpl::~CInputImpl()
{
    // TODO: Implement
}

void CInput::Init()
{
    CLogger::Info("Initializing input system");
    PROFILE_FUNCTION();
    mImplementation = new CInputImpl;
}

void CInput::Shutdown()
{
    CLogger::Info("Shutting down input system");
    PROFILE_FUNCTION();
    delete mImplementation;
}
