//
// Created by Diego Santos Seabra on 21/08/22.
//

#include "Texture.h"
#include "VulkanDevice.h"

//
// Constructor/Destructor
//

void Texture::Create(const std::string& imagePath)
{
    CreateTextureImage(imagePath);
    CreateImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
    CreateTextureSampler();
}

void Texture::Destroy()

{
    vkDestroySampler(VulkanDevice::GetDevice(), sampler, nullptr);
    vkDestroyImageView(VulkanDevice::GetDevice(), imageView, nullptr);

    vkDestroyImage(VulkanDevice::GetDevice(), image, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), imageMemory, nullptr);
}

//
// Implementation
//

void Texture::CreateTextureImage(const std::string &imagePath)
{
    // reading the image with the stbi library
    pixels = stbi_load(imagePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    // calculating the size of the image (in vulkan terms)
    VkDeviceSize imageSize = width * height * 4;

    // checking if the image data was filled (e.g. we have pixel data)
    if (!pixels)
    {
        Logger::Error("Failed to load texture image!", "");
        throw std::runtime_error("failed to load texture image"); // REVIEW: Here we'll probably just want to handle the error instead of stopping the engine!
    }

    // creating a buffer to upload the texure to the GPU
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VulkanDevice::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                               stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanDevice::GetDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(VulkanDevice::GetDevice(), stagingBufferMemory);

    // texture image is now mapped on the GPU so we don't need it's data representation anymore
    stbi_image_free(pixels);

    // creating and uploading the image to the GPU
    CreateImage(width, height, VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                         VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                image, imageMemory);

    TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    CopyBufferToImage(stagingBuffer, image,
                      static_cast<uint32_t>(width),
                      static_cast<uint32_t>(height));

    TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // destroying stuff because now the image is in the GPU
    vkDestroyBuffer(VulkanDevice::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), stagingBufferMemory, nullptr);
}

void Texture::CreateTextureSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST; // if we want to use pixel art, we use VK_FILTER_NEAREST
    samplerInfo.minFilter = VK_FILTER_NEAREST; // if we want to use pixel art, we use VK_FILTER_NEAREST
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE; // texels are addressed using the [0, 1) range on all axes
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    VK_CHECK(vkCreateSampler(VulkanDevice::GetDevice(), &samplerInfo, nullptr, &sampler));
}

//
// Private
//

void Texture::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
{
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
    allocInfo.memoryTypeIndex = VulkanDevice::FindMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(VulkanDevice::GetDevice(), &allocInfo, nullptr, &imageMemory));

    vkBindImageMemory(VulkanDevice::GetDevice(), image, imageMemory, 0);
}

void Texture::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags)
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

    VK_CHECK(vkCreateImageView(VulkanDevice::GetDevice(), &viewInfo, nullptr, &imageView));
}

void Texture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                                             VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = VulkanDevice::BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{}; // barriers are used for synchronization purposes
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0; // TODO
    barrier.dstAccessMask = 0; // TODO

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else
    {
        throw std::runtime_error("unsupported layout transition");
    }

    vkCmdPipelineBarrier(commandBuffer,
                         sourceStage, destinationStage,
                         0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);

    VulkanDevice::EndSingleTimeCommands(commandBuffer);
}

void Texture::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = VulkanDevice::BeginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
            width,
            height,
            1
    };

    vkCmdCopyBufferToImage(commandBuffer,
                           buffer,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &region);

    VulkanDevice::EndSingleTimeCommands(commandBuffer);
}


