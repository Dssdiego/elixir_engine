//
// Created by Diego S. Seabra on 23/08/22.
//

#ifndef VULKAN_ENGINE_SPRITE_H
#define VULKAN_ENGINE_SPRITE_H

#include "Texture.h"
#include "../common/GameObject.h"

class Sprite : public GameObject
{
public:
    Sprite();
    Sprite(uint32_t line, uint32_t column);
    ~Sprite();

private:
    uint32_t line; // line reference in the sprite atlas
    uint32_t column; // column reference in the sprite atlas
};


#endif //VULKAN_ENGINE_SPRITE_H
