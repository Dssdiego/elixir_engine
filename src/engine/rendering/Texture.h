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
    Texture(const std::string &imagePath);
    ~Texture();

    void Create(const std::string& imagePath);

private:
    int width, height, channels;
    stbi_uc* pixels;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
};
