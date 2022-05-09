//
// Created by Diego S. Seabra on 14/03/22.
//

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
            CVulkanRenderer::Init();
            break;

        case DIRECTX:
            CDirectXRenderer::Init();

        case OPENGL:
            break;
    }
}

void CRenderer::Draw()
{
    // FIXME: Using vulkan as default backend for now. This should be dynamic and follow the current backend
    CVulkanRenderer::Draw();
}

void CRenderer::Shutdown()
{
    CLogger::Info("Shutting down renderer");
    // FIXME: Using vulkan as default backend for now. This should be dynamic and follow the current backend
    CVulkanRenderer::Shutdown();
}
