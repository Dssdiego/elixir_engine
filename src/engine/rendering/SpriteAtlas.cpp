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

glm::vec2 SpriteAtlas::GetTextureSize()
{
    return texture->Size();
}

glm::vec4 SpriteAtlas::GetUVCoordinate(uint32_t line, uint32_t column)
{
    auto texSize = texture->Size();
    auto lineIncrement = (float) tileSize / texSize.y;
    auto columnIncrement = (float) tileSize / texSize.x;

    auto uvStartX = (float) column * columnIncrement;
    auto uvStartY = (float) line * lineIncrement;

    auto uvEndX = (float) (column + 1) * columnIncrement;
    auto uvEndY = (float) (line + 1) * lineIncrement;

    return {uvStartX, uvStartY, uvEndX, uvEndY};
}
