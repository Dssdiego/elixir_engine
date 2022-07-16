//
// Created by Diego S. Seabra on 09/07/22.
//

#include "VulkanSwapchain.h"

void VulkanSwapchain::Init()
{
    CreateSwapChain();
//    CreateImageViews();
//    CreateRenderPass();
//    CreateDepthResources();
//    CreateFramebuffers();
//    CreateSyncObjects();
}

void VulkanSwapchain::CreateSwapChain()
{
    // TODO: CreateSwapChain
//    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vkContext.physicalDevice);
//
//    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
//    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
//    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);
//
//    imageCount = swapChainSupport.capabilities.minImageCount + 1; // one more image to have "room" for more processing
//    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
//    {
//        // making sure we don't exceed the maximum number of images in the swap chain
//        vkContext.swapChainImageCount = swapChainSupport.capabilities.maxImageCount;
//    }
//    std::cout << "Minimum image count in the swap chain: " << imageCount << std::endl;
//
//    VkSwapchainCreateInfoKHR createInfo{};
//    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//    createInfo.surface = vkSurface; // tying our surface to the swap chain
//    createInfo.minImageCount = imageCount;
//    createInfo.imageFormat = surfaceFormat.format;
//    createInfo.imageColorSpace = surfaceFormat.colorSpace;
//    createInfo.imageExtent = extent;
//    createInfo.imageArrayLayers = 1; // the amount of layers each image consists of (always 1 unless we are developing a stereoscopic 3D app)
//
//    // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - This is a color image we're rendering into
//    // VK_IMAGE_USAGE_TRANSFER_SRC_BIT - We'll be copying the image somewhere (screenshot, postProcess)
//    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//
//    uint32_t queueFamilyIndices[] = {vkContext.graphicsFamilyIdx.value(), vkContext.presentFamilyIdx.value()};
//
//    // specifying how to handle swap chain images that will be used across multiple queue families (graphics and presentation)
//    // in our case, we'll be drawing on the images in the swap chain from the graphics queue and then
//    //    submitting them on the presentation queue
//    if (vkContext.graphicsFamilyIdx != vkContext.presentFamilyIdx)
//    {
//        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//        createInfo.queueFamilyIndexCount = 2;
//        createInfo.pQueueFamilyIndices = queueFamilyIndices;
//    } else {
//        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//        createInfo.queueFamilyIndexCount = 0; // optional
//        createInfo.pQueueFamilyIndices = nullptr; // optional
//    }
//
//    // we don't want any pre-transformation on the images for now
//    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
//
//    // compositeAlpha = blending with other windows in the window system. So we will ignore the alpha channel (aka opaque)
//    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//
//    createInfo.presentMode = presentMode;
//    createInfo.clipped = VK_TRUE; // we don't care for pixels that are obscured (ex.: another window in front of our own)
//    createInfo.oldSwapchain = VK_NULL_HANDLE; // for now we assume that we'll be creating only one swap chain.
//    // NOTE: Only one swap chain is not the best option because we'd like the screen to be resized, so in the future we'll probably recreate the swap chain ;)
//
//    // effectively creating the swap chain
//    VK_CHECK(vkCreateSwapchainKHR(vkContext.logicalDevice, &createInfo, nullptr, &vkSwapChain));
//
//    // retrieve swap chain images from the logical device
//    vkGetSwapchainImagesKHR(vkContext.logicalDevice, vkSwapChain, &vkContext.swapChainImageCount, nullptr);
//    vkSwapChainImages.resize(vkContext.swapChainImageCount);
//    vkGetSwapchainImagesKHR(vkContext.logicalDevice, vkSwapChain, &vkContext.swapChainImageCount, vkSwapChainImages.data());
//
//    vkSwapChainImageFormat = surfaceFormat.format;
//    vkSwapChainExtent = extent;
//
//    Logger::Debug("Swap chain created");
}
