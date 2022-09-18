//
// Created by Diego Santos Seabra on 21/08/22.
//

#pragma once

#include "stb/stb_image.h"

#include "vulkan/VulkanDevice.h"
#include "vulkan/VulkanImage.h"

#include <string>
#include <vulkan/vulkan.h>

class Texture
{
public:
    void Create(const std::string& imagePath);
    VkDescriptorImageInfo DescriptorInfo();
    void Destroy();

private:
    int width, height, channels;
    stbi_uc* pixels;

    // FIXME: We don't need a sampler for every texture. Only a few of them should be enough
    VkSampler sampler;

    VkImage image;
    VkImageView imageView;
    VkImageLayout imageLayout;

    VkDeviceMemory imageMemory;

    // Vulkan image creation/definition
    void CreateTextureImage(const std::string &imagePath);
    void CreateTextureSampler();
};
