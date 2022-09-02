//
// Created by Diego S. Seabra on 01/09/22.
//

#ifndef VULKAN_ENGINE_CAMERA_H
#define VULKAN_ENGINE_CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class Camera
{
public:
    static void SetOrtographicProjection(float left, float right, float top, float bottom, float near, float far);
    static void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

    static const glm::mat4 &GetProjection();

private:
    inline static glm::mat4 projectionMatrix{1.f};
};


#endif //VULKAN_ENGINE_CAMERA_H
