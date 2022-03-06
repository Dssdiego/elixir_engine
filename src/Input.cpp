//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"

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
    mImplementation = new CInputImpl;
}

void CInput::Shutdown()
{
    delete mImplementation;
}
