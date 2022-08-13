//
// Created by Diego S. Seabra on 14/03/22.
//

#include <Tracy.hpp>
#include "Renderer.h"
#include "DirectXRenderer.h"
#include "../profiling/Logger.h"
#include "VulkanRenderer.h"

// TODO: Allow to change backends

void Renderer::Init(GraphicsBackend backend)
{
    Logger::Info("Initializing renderer");

    switch (backend)
    {
        case VULKAN:
            Logger::Info("Renderer is using VULKAN");
            CVulkanRenderer::Init();
            break;

#ifdef _WIN32
        case DIRECTX:
            Logger::Info("Renderer is using DIRECTX");
            DirectXRenderer::Init();
            break;
#endif

        case OPENGL:
            Logger::Info("Renderer is using OPENGL");
            break;
    }
}

void Renderer::Draw()
{
    ZoneScopedC(0x9b59b6);
    // FIXME: Using vulkan as default backend for now. This should be dynamic and choose the correct backend
//    CVulkanRenderer::Draw();
}

void Renderer::Shutdown()
{
    Logger::Info("Shutting down renderer");
    // FIXME: Using vulkan as default backend for now. This should be dynamic and choose the correct backend
    CVulkanRenderer::Shutdown();
}
