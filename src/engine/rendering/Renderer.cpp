//
// Created by Diego S. Seabra on 14/03/22.
//

#include <Tracy.hpp>
#include "Renderer.h"
#include "DirectXRenderer.h"
#include "../profiling/Logger.h"
#include "VulkanRenderer.h"

// TODO: Allow to change backends

void CRenderer::Init(SBackend backend)
{
    CLogger::Info("Initializing renderer");

    switch (backend)
    {
        case VULKAN:
            CLogger::Info("Renderer is using VULKAN");
            CVulkanRenderer::Init();
            break;

#ifdef _WIN32
        case DIRECTX:
            CLogger::Info("Renderer is using DIRECTX");
            CDirectXRenderer::Init();
            break;
#endif

        case OPENGL:
            CLogger::Info("Renderer is using OPENGL");
            break;
    }
}

void CRenderer::Draw()
{
    ZoneScopedC(0x9b59b6);
    // FIXME: Using vulkan as default backend for now. This should be dynamic and follow the current backend
    CVulkanRenderer::Draw();
}

void CRenderer::Shutdown()
{
    CLogger::Info("Shutting down renderer");
    // FIXME: Using vulkan as default backend for now. This should be dynamic and follow the current backend
    CVulkanRenderer::Shutdown();
}
