//
// Created by Diego Santos Seabra on 21/08/22.
//

#include "Texture.h"
#include "VulkanDevice.h"

void Texture::Create(const std::string& imagePath)
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
    VulkanDevice::CreateImage(width, height, VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                         VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                textureImage, textureImageMemory);

    VulkanDevice::TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VulkanDevice::CopyBufferToImage(stagingBuffer, textureImage,
                      static_cast<uint32_t>(width),
                      static_cast<uint32_t>(height));

    VulkanDevice::TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // destroying stuff because now the image is in the GPU
    vkDestroyBuffer(VulkanDevice::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), stagingBufferMemory, nullptr);
}
