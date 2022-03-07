//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"
#include "Profiler.h"

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
    PROFILE_FUNCTION();
    mImplementation = new CInputImpl;
}

void CInput::Shutdown()
{
    PROFILE_FUNCTION();
    delete mImplementation;
}
