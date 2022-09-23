//
// Created by Diego Santos Seabra on 21/08/22.
//

#include "Texture.h"
#include "vulkan/VulkanBuffer.h"

//
// Constructor/Destructor
//

void Texture::Create(const std::string& imagePath)
{
    CreateTextureImage(imagePath);
    VulkanImage::CreateImageView(image, imageView, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
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
    uint32_t imageCount = sizeof(pixels[0]);

    // checking if the image data was filled (e.g. we have pixel data)
    if (!pixels)
    {
        Logger::Error("Failed to load texture image!", "");
        throw std::runtime_error("failed to load texture image"); // REVIEW: Here we'll probably just want to handle the error instead of stopping the engine!
    }

    // creating a buffer to upload the texure to the GPU
    VulkanBuffer stagingBuffer(
            imageSize,
            imageCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stagingBuffer.Map();
    stagingBuffer.WriteToBuffer((void *) pixels);

    // texture image is now mapped on the GPU so we don't need it's data representation anymore
    stbi_image_free(pixels);

    // creating and uploading the image to the GPU
    VulkanImage::CreateImage(width, height, VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                         VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                image, imageMemory);

    VulkanImage::TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM,
                          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VulkanImage::CopyBufferToImage(stagingBuffer.GetBuffer(), image,
                      static_cast<uint32_t>(width),
                      static_cast<uint32_t>(height));

    VulkanImage::TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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

    Logger::Debug("Created texture sampler");
}

VkDescriptorImageInfo Texture::DescriptorInfo()
{
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;
    return imageInfo;
}

glm::vec2 Texture::Size()
{
    return {width, height};
}
