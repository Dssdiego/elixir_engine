//
// Created by Diego S. Seabra on 22/09/22.
//

#ifndef VULKAN_ENGINE_SPRITEATLAS_H
#define VULKAN_ENGINE_SPRITEATLAS_H

#include <iostream>
#include "Texture.h"
#include <imgui.h>
#include <imgui_impl_vulkan.h>

// NOTE: We are only using a single texture for all engine/game sprites because pixel art is very small
//       and a 4k texture is enough to hold a lot of images ;)
class SpriteAtlas
{
public:
    static void CreateTexture();
    static void DestroyTexture();
    static VkDescriptorImageInfo GetTextureDescriptorInfo();
    static void CreateImGuiImage();
    static ImTextureID GetImGuiImage();
    static glm::vec2 GetTextureSize();
    static glm::vec4 GetUVCoordinate(uint32_t line, uint32_t column);

private:
    inline static std::unique_ptr<Texture> texture = nullptr;
    inline static ImTextureID imguiTexture = nullptr;
    inline static uint32_t tileSize = 16; // size of the tile in the atlas (we are using 32x32 pixel sprites)
};


#endif //VULKAN_ENGINE_SPRITEATLAS_H
