//
// Created by Diego S. Seabra on 08/06/22.
//

#ifndef VULKAN_ENGINE_GAMEOBJECT_H
#define VULKAN_ENGINE_GAMEOBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include "../rendering/shapes/Shape.h"
#include "../rendering/SpriteAtlas.h"

struct Transform2D {
    glm::vec3 position{0.f, 0.f, 0.f};
    glm::vec3 scale{1.f, 1.f, 1.f};
    float rotation = 0.f; // NOTE: Degrees

    glm::mat4 mat4()
    {
        auto transform = glm::translate(glm::mat4{1.f}, glm::vec3{position[0], position[1], position[2]});
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3{scale[0], scale[1], scale[2]});
        return transform;
    }
};

class GameObject
{
public:
    uint32_t id = 0;
    std::string name = "";

    std::string pipeline = "";
    Transform2D transform{};
    Shape shape{ShapeType::Quad}; // TODO: Change this so that we can pass any shape we want ;)
    Color color = Color::White;

    static GameObject Create() {
        return GameObject{};
    }

protected:
    GameObject(){}
};


#endif //VULKAN_ENGINE_GAMEOBJECT_H
