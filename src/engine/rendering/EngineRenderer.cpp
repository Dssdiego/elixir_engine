//
// Created by Diego S. Seabra on 21/07/22.
//

#include "EngineRenderer.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
EngineRendererImpl* mEngineRendererImpl = nullptr;

//
// Initialization/Destruction
//

void EngineRenderer::Init()
{

}

void EngineRenderer::Shutdown()
{

}

//
// Implementation
//

VkCommandBuffer EngineRenderer::BeginFrame()
{
    return nullptr;
}

void EngineRenderer::EndFrame()
{

}
