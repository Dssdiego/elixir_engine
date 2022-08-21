//
// Created by Diego S. Seabra on 08/06/22.
//

#ifndef VULKAN_ENGINE_GAMEOBJECT_H
#define VULKAN_ENGINE_GAMEOBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include "../rendering/shapes/Shape.h"
#include "../rendering/shapes/Triangle.h"

struct Transform2D {
    glm::vec2 position{};
    glm::vec2 scale{1.f, 1.f};
    float rotation; // REVIEW: In degrees? Radians? Euler angles? What is more intuitive?

    // 2D Transformation matrix (for rotationa and scale calculations)
    glm::mat2 mat2() {
        const float sine = glm::sin(rotation);
        const float cosine = glm::cos(rotation);
        glm::mat2 rotMatrix{{cosine, sine}, {-sine, cosine}};

        glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
        return rotMatrix * scaleMat;
    };
};

class GameObject
{
public:
    Transform2D transform{};
    Shape shape{}; // TODO: Change this so that we can pass any shape we want ;)
    glm::vec3 color{};

    uint32_t GetId() { return id; }

    static GameObject Create() {
        return GameObject{id++};
    }

private:
    inline static uint32_t id = 0;

    GameObject(uint32_t objId){ id = objId; }
};


#endif //VULKAN_ENGINE_GAMEOBJECT_H
