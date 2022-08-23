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

struct Transform2D {
    float position[3];
    float scale[3];
//    glm::vec2 position{};
//    glm::vec2 scale{1.f, 1.f};
    float rotation; // REVIEW: In degrees? Radians? Euler angles? What is more intuitive?

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

    Transform2D transform{};
    Shape shape{ShapeType::Triangle}; // TODO: Change this so that we can pass any shape we want ;)
    float color[4]{};
//    glm::vec3 color{};

//    uint32_t GetId() { return id; }

    static GameObject Create() {
        return GameObject{};
    }

private:
    GameObject(){}
};


#endif //VULKAN_ENGINE_GAMEOBJECT_H
