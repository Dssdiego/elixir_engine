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

    // REVIEW: Recreate swapchain on renderer init?
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
    mEngineRendererImpl->BeginFrame();
}

void EngineRenderer::EndFrame()
{
    mEngineRendererImpl->EndFrame();

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

VkCommandBuffer EngineRendererImpl::GetCurrentCommandBuffer()
{
    // REVIEW: Assert the frame is already started? We can't get command buffer when a frame is not in progress!
    return commandBuffers[currentFrameIndex];
}

VkCommandBuffer EngineRendererImpl::BeginFrame()
{
    assert(!frameHasStarted && "Can't call BeginFrame() while a frame is already in progress");

    auto result = VulkanSwapchain::AcquireNextImage(&mEngineRendererImpl->currentImageIdx);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        VulkanSwapchain::Recreate();
        return nullptr;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        // REVIEW: Should this be a runtime error?
        Logger::Error("Failed to acquire swap chain image", "");
        throw std::runtime_error("failed to acquire swap chain image");
    }

    frameHasStarted = true;

    auto commandBuffer = GetCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    // REVIEW: Which flag to use here? Do we actually need a flag?
//    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//    VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT specifies that each recording of the command buffer will only be submitted once, and the command buffer will be reset and recorded again between each submission.
//    VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT specifies that a secondary command buffer is considered to be entirely inside a render pass. If this is a primary command buffer, then this bit is ignored.
//    VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT specifies that a command buffer can be resubmitted to a queue while it is in the pending state, and recorded into multiple primary command buffers.

    VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    return commandBuffer;
}

void EngineRendererImpl::EndFrame()
{
    assert(frameHasStarted && "Can't call EndFrame() while frame is not in progress");

    auto commandBuffer = GetCurrentCommandBuffer();
    VK_CHECK(vkEndCommandBuffer(commandBuffer));

    auto result = VulkanSwapchain::SubmitCommandBuffers(&commandBuffer, &currentImageIdx);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        VulkanSwapchain::Recreate();
    } else if (result != VK_SUCCESS)
    {
        std::string err = "Failed to present swap chain image";

        Logger::Error(err, "");
        throw std::runtime_error(err);
    }

    frameHasStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % VulkanSwapchain::GetNumberOfFramesInFlight();
}
