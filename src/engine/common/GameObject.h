//
// Created by Diego S. Seabra on 08/06/22.
//

#ifndef VULKAN_ENGINE_GAMEOBJECT_H
#define VULKAN_ENGINE_GAMEOBJECT_H

#include <glm/vec2.hpp>

struct Transform {
    glm::vec2 position;
    uint32_t rotation; // REVIEW: In degrees? Euler angles? What is more intuitive?
    glm::vec2 scale;
};

class GameObject
{
public:
    virtual void Init() {};
    virtual void Update() {};
    virtual void Draw() {};
};


#endif //VULKAN_ENGINE_GAMEOBJECT_H
