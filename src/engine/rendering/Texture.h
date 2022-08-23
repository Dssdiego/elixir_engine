//
// Created by Diego Santos Seabra on 21/08/22.
//

#pragma once

#include "stb/stb_image.h"
#include <string>
#include <vulkan/vulkan.h>

class Texture
{
public:
    void Create(const std::string& imagePath);
    void Destroy();

private:
    int width, height, channels;
    stbi_uc* pixels;

    VkSampler sampler;

    VkImage image;
    VkImageView imageView;
    VkImageLayout imageLayout;

    VkDeviceMemory imageMemory;

    // Vulkan image creation/definition
    void CreateTextureImage(const std::string &imagePath);
    void CreateTextureSampler();

    // Helper Methods
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};
