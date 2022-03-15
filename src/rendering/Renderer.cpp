//
// Created by Diego S. Seabra on 14/03/22.
//

#include "Renderer.h"
#include "DirectXRenderer.h"
#include "../profiling/Logger.h"

// TODO: Make possibility to change backends

void CRenderer::Init(SBackend backend)
{
    CLogger::Info("Initializing renderer");
    CDirectXRenderer::Init();

//    switch (backend)
//    {
//        case VULKAN:
//            break;
//
//        case DIRECTX:
//            CDirectXRenderer::Init();
//
//        case OPENGL:
//            break;
//    }
}

void CRenderer::Draw()
{

}

void CRenderer::Shutdown()
{
    CLogger::Info("Shutting down renderer");
    CDirectXRenderer::Shutdown();
}
