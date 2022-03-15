//
// Created by Diego S. Seabra on 14/03/22.
//

#include "DirectXRenderer.h"

CDirectXRendererImpl* mImplementation = nullptr;

void CDirectXRenderer::Init()
{
    mImplementation = new CDirectXRendererImpl;
}

void CDirectXRenderer::Shutdown()
{
    delete mImplementation;
}

CDirectXRendererImpl::CDirectXRendererImpl()
{
    CreateFactory();
}

CDirectXRendererImpl::~CDirectXRendererImpl()
{
    // destroy the factory
    mFactory->Release();
}

// Factories are the entry point to the DirectX 12 API,
// and will allow you to find adapters that you can use to execute DirectX 12 commands.
void CDirectXRendererImpl::CreateFactory()
{
    if FAILED(CreateDXGIFactory2(mDxgiFactoryFlags, IID_PPV_ARGS(&mFactory)))
    {
        throw std::runtime_error("DIRECTX: Failed to create factory");
    }
}
