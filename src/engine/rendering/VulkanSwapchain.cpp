//
// Created by Diego S. Seabra on 09/07/22.
//

#include "VulkanSwapchain.h"
#include "VulkanDevice.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
VulkanSwapChainImpl* mVulkanSwapChainImpl = nullptr;

//
// Initialization/Destruction
//

void VulkanSwapchain::Init()
{
    mVulkanSwapChainImpl = new VulkanSwapChainImpl;
}

void VulkanSwapchain::Shutdown()
{
    delete mVulkanSwapChainImpl;
}

uint32_t VulkanSwapchain::GetImageCount()
{
    return mVulkanSwapChainImpl->GetImageCount();
}

uint32_t VulkanSwapchain::GetNumberOfFramesInFlight()
{
    return mVulkanSwapChainImpl->MAX_FRAMES_IN_FLIGHT;
}

void VulkanSwapchain::Recreate()
{
    // when trying to recreate the swapchain, check if it already exists. if exists, we shut it down and we create it again
    if (mVulkanSwapChainImpl->swapChain != nullptr)
    {
        Shutdown();
        Init();
    } else {
        // if it doesn't exists, we create it from scratch
        Init();
    }
}

VkResult VulkanSwapchain::AcquireNextImage(uint32_t *imageIndex)
{
    return mVulkanSwapChainImpl->AcquireNextImage(imageIndex);
}

//
// Implementation
//

VulkanSwapChainImpl::VulkanSwapChainImpl()
{
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateDepthResources();
    CreateFrameBuffers();
    CreateSyncObjects();
}

VulkanSwapChainImpl::~VulkanSwapChainImpl()
{
    Logger::Debug("Destroying swapchain image views");
    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(VulkanDevice::GetDevice(), imageView, nullptr);
    }
    swapChainImageViews.clear();

    Logger::Debug("Destroying swapchain");
    if (swapChain != nullptr)
    {
        vkDestroySwapchainKHR(VulkanDevice::GetDevice(), swapChain, nullptr);
        swapChain = nullptr;
    }

    // TODO: Destroy depth images (image view, image and free memory)
    Logger::Debug("Destroying depth image");
    vkDestroyImageView(VulkanDevice::GetDevice(), depthImageView, nullptr);
    vkDestroyImage(VulkanDevice::GetDevice(), depthImage, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), depthImageMemory, nullptr);

    Logger::Debug("Destroying framebuffers");
    for (auto frameBuffer : swapChainFrameBuffers)
    {
        vkDestroyFramebuffer(VulkanDevice::GetDevice(), frameBuffer, nullptr);
    }

    Logger::Debug("Destroying renderpass");
    vkDestroyRenderPass(VulkanDevice::GetDevice(), renderPass, nullptr);

    // TODO: Cleanup synchonization objects (semaphores and fences)
    Logger::Debug("Destroying sync objects");
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(VulkanDevice::GetDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(VulkanDevice::GetDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(VulkanDevice::GetDevice(), inFlightFences[i], nullptr);
    }
}

void VulkanSwapChainImpl::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = VulkanDevice::GetSwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // one more image to have "room" for more processing
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        // making sure we don't exceed the maximum number of images in the swap chain
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    std::cout << "Minimum image count in the swap chain: " << imageCount << std::endl;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = VulkanDevice::GetSurface(); // tying our surface to the swap chain
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // the amount of layers each image consists of (always 1 unless we are developing a stereoscopic 3D app)

    // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - This is a color image we're rendering into
    // VK_IMAGE_USAGE_TRANSFER_SRC_BIT - We'll be copying the image somewhere (screenshot, postProcess)
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    uint32_t queueFamilyIndices[] = { VulkanDevice::GetGraphicsQueueFamilyIdx(), VulkanDevice::GetPresentQueueFamilyIdx() };

    // specifying how to handle swap chain images that will be used across multiple queue families (graphics and presentation)
    // in our case, we'll be drawing on the images in the swap chain from the graphics queue and then
    //    submitting them on the presentation queue
    // REVIEW: Is this correct?
    if (VulkanDevice::GetGraphicsQueueFamilyIdx() != VulkanDevice::GetPresentQueueFamilyIdx())
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // optional
        createInfo.pQueueFamilyIndices = nullptr; // optional
    }

    // we don't want any pre-transformation on the images for now
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    // compositeAlpha = blending with other windows in the window system. So we will ignore the alpha channel (aka opaque)
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE; // we don't care for pixels that are obscured (ex.: another window in front of our own)
    createInfo.oldSwapchain = VK_NULL_HANDLE; // for now we assume that we'll be creating only one swap chain.
    // NOTE: Only one swap chain is not the best option because we'd like the screen to be resized, so in the future we'll probably recreate the swap chain ;)

    // effectively creating the swap chain
    VK_CHECK(vkCreateSwapchainKHR(VulkanDevice::GetDevice(), &createInfo, nullptr, &swapChain));

    // retrieve swap chain images from the logical device
    vkGetSwapchainImagesKHR(VulkanDevice::GetDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(VulkanDevice::GetDevice(), swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    Logger::Debug("Swap chain created");
}

void VulkanSwapChainImpl::CreateImageViews()
{
    // resizing the list to fit all of the image views
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    std::cout << "# of image views created: " << swapChainImageViews.size() << std::endl;
}

void VulkanSwapChainImpl::CreateRenderPass()
{
    // for now, just a single color buffer attachment represented by one of the images from the swap chain
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat; // matching the format of the swap chain images
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // matching the format of the swap chain images
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // clear the framebuffer before drawing a new frame
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // rendered contents will be stored in memory
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // doing nothing with stencil buffers for now, so we don't care what is in the buffer
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // doing nothing with stencil buffers for now, so we don't care what is in the buffer
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // layout of the image before the render pass begins
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // layout to automatically transition the image to when the render pass finishes

    // we'll stick to a single subpass for now (so index 0)
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // depth
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // wait for the swap chain to finish reading before we can access it
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    // effectively creating the render pass
    VK_CHECK(vkCreateRenderPass(VulkanDevice::GetDevice(), &renderPassInfo, nullptr, &renderPass));

    Logger::Debug("Render pass created");
}

void VulkanSwapChainImpl::CreateDepthResources()
{
    // REVIEW: Use multiple depth buffers/images? Ex.: One depth buffer per framebuffer?
    //         How it would change on our render pass?
    VkFormat depthFormat = FindDepthFormat();

    CreateImage(swapChainExtent.width, swapChainExtent.height,
                depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage,
                depthImageMemory);

    depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void VulkanSwapChainImpl::CreateFrameBuffers()
{
    // resizing the container to hold all of the framebuffers
    // REVIEW: Use swapchain image count here?
    swapChainFrameBuffers.resize(swapChainImageViews.size());

    // iterate through the image views and create framebuffers from them
    // REVIEW: Use swapchain image count here?
    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
                swapChainImageViews[i],
                depthImageView // REVIEW: Pass multiple depth buffers here if we are using them
        };

        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = renderPass;
        frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        frameBufferInfo.pAttachments = attachments.data();
        frameBufferInfo.width = swapChainExtent.width;
        frameBufferInfo.height = swapChainExtent.height;
        frameBufferInfo.layers = 1; // out swap chain contains single images, so only 1 layer

        VK_CHECK(vkCreateFramebuffer(VulkanDevice::GetDevice(), &frameBufferInfo, nullptr, &swapChainFrameBuffers[i]));
    }

    Logger::Debug("Framebuffers created");
}

void VulkanSwapChainImpl::CreateSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(GetImageCount(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // REVIEW: Why use the signaled bit?

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VK_CHECK(vkCreateSemaphore(VulkanDevice::GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]));
        VK_CHECK(vkCreateSemaphore(VulkanDevice::GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]));
        VK_CHECK(vkCreateFence(VulkanDevice::GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]));
    }

    Logger::Debug("Created sync objects (semaphores and fences)");
}

VkResult VulkanSwapChainImpl::AcquireNextImage(uint32_t *imageIndex)
{
    vkWaitForFences(
            VulkanDevice::GetDevice(),
            1,
            &inFlightFences[currentFrame],
            VK_TRUE,
            std::numeric_limits<uint64_t>::max());

    VkResult result = vkAcquireNextImageKHR(
            VulkanDevice::GetDevice(),
            swapChain,
            std::numeric_limits<uint64_t>::max(),
            imageAvailableSemaphores[currentFrame],  // must be a not signaled semaphore
            VK_NULL_HANDLE,
            imageIndex);

    return result;
}

//
// Helpers
//

VkImageView VulkanSwapChainImpl::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.format = format;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    VK_CHECK(vkCreateImageView(VulkanDevice::GetDevice(), &viewInfo, nullptr, &imageView));

    return imageView;
}

VkSurfaceFormatKHR VulkanSwapChainImpl::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    VkSurfaceFormatKHR result;

    // if vulkan returned an unknown format, then we just force what we want.
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        result.format = VK_FORMAT_B8G8R8A8_UNORM;
        result.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return result;
    }

    // favor 32 bit rgba and srgb nonlinear colorspace
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // if all else fails, just return what's available
    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChainImpl::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    // checking if triple buffering is available
    for (const auto& availablePresentMode: availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            Logger::Debug("Present Mode: MAILBOX");
            return availablePresentMode;
        }
    }

    // if triple buffering is not available, use Vulkan's default swap chain queue (aka VSync)
    Logger::Debug("Present Mode: V-SYNC");
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChainImpl::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(Window::GetWindow(), &width, &height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width =
                std::max(capabilities.minImageExtent.width,
                         std::min(capabilities.maxImageExtent.width,
                                  actualExtent.width));

        actualExtent.height =
                std::max(capabilities.minImageExtent.height,
                         std::min(capabilities.maxImageExtent.height,
                                  actualExtent.height));

        return actualExtent;
    }
}

VkFormat VulkanSwapChainImpl::FindDepthFormat()
{
    return FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat VulkanSwapChainImpl::FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                                  VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(VulkanDevice::GetPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    Logger::Error("failed to find supported format", "");
    throw std::runtime_error("failed to find supported format");
}

void VulkanSwapChainImpl::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                                      VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
                                      VkDeviceMemory &imageMemory)
{
    // REVIEW: I stopped here
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1; // how many texels are on each axis of the image
    imageInfo.mipLevels = 1; // not using mipmapping for now
    imageInfo.arrayLayers = 1; // texture is not an array
    imageInfo.format = format; // same as the pixels in the buffer
    imageInfo.tiling = tiling; // texels are laid out in an implementation defined order for optimal access
    // NOTE: tiling can't be changed at a later time, layout can
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT; // related to multisampling
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // the image will only be used by one queue family
    imageInfo.flags = 0; // optional

    VK_CHECK(vkCreateImage(VulkanDevice::GetDevice(), &imageInfo, nullptr, &image));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(VulkanDevice::GetDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(VulkanDevice::GetDevice(), &allocInfo, nullptr, &imageMemory));

    vkBindImageMemory(VulkanDevice::GetDevice(), image, imageMemory, 0);

}

uint32_t VulkanSwapChainImpl::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    // querying info about the available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(VulkanDevice::GetPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find a suitable memory type");
}

uint32_t VulkanSwapChainImpl::GetImageCount()
{
    return swapChainImages.size();
}


