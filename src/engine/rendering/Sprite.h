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
    ~Sprite();

private:
    Texture texture{};
};


#endif //VULKAN_ENGINE_SPRITE_H
