//
// Created by Diego S. Seabra on 22/09/22.
//

#include "SpriteAtlas.h"

void SpriteAtlas::CreateTexture()
{
    texture = std::make_unique<Texture>();
    texture->Create("assets/textures/game_atlas.png");
}

void SpriteAtlas::DestroyTexture()
{
    texture->Destroy();
    texture = nullptr;
}

VkDescriptorImageInfo SpriteAtlas::GetTextureDescriptorInfo()
{
    return texture->DescriptorInfo();
}
