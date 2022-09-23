//
// Created by Diego S. Seabra on 22/09/22.
//

#ifndef VULKAN_ENGINE_SPRITEATLAS_H
#define VULKAN_ENGINE_SPRITEATLAS_H

#include <iostream>
#include "Texture.h"

// NOTE: We are only using a single texture for all engine/game sprites because pixel art is very small
//       and a 4k texture is enough to hold a lot of images ;)
class SpriteAtlas
{
public:
    static void CreateTexture();
    static void DestroyTexture();
    static VkDescriptorImageInfo GetTextureDescriptorInfo();
//    static glm::vec2 GetTextureSize();

private:
    inline static std::unique_ptr<Texture> texture = nullptr;
};


#endif //VULKAN_ENGINE_SPRITEATLAS_H
