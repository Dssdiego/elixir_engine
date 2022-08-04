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
    Logger::Info("Initializing engine renderer");

    VulkanDevice::Init();
    VulkanSwapchain::Init();
}

void EngineRenderer::Shutdown()
{
    Logger::Info("Shutting down engine renderer");

    VulkanSwapchain::Shutdown();
    VulkanDevice::Shutdown();
}

//
// External
//

VkCommandBuffer EngineRenderer::BeginFrame()
{
    return nullptr;
}

void EngineRenderer::EndFrame()
{

}

//
// Implementation
//

void EngineRendererImpl::CreateCommandBuffers()
{
    // REVIEW: This is probably wrong since we are creating the sync objects in the swap chain!
    commandBuffers.resize(VulkanSwapchain::GetNumberOfFramesInFlight());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = VulkanDevice::GetCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    VK_CHECK(vkAllocateCommandBuffers(VulkanDevice::GetDevice(), &allocInfo, commandBuffers.data()));

    Logger::Debug("Created command buffers");
}

void EngineRendererImpl::FreeCommandBuffers()
{
    vkFreeCommandBuffers(
            VulkanDevice::GetDevice(),
            VulkanDevice::GetCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
    commandBuffers.clear();
}
