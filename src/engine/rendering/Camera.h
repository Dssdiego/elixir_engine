//
// Created by Diego S. Seabra on 01/09/22.
//

#ifndef VULKAN_ENGINE_CAMERA_H
#define VULKAN_ENGINE_CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "../core/Time.h"

class Camera
{
public:
    static void SetOrtographicProjection(float left, float right, float top, float bottom, float near, float far);
    static void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

    static void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));
    static void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));
    static void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

    inline static glm::vec3 GetWorldPosition() { return worldPosition; }
    inline static glm::vec3 *GetWorldPositionRef() { return &worldPosition; }
    inline static void SetWorldPosition(glm::vec3 position) { worldPosition = position; UpdateCameraPosition(); }
    inline static void UpdateCameraPosition() { SetViewDirection(worldPosition, worldDirection); }

    inline static glm::vec3 GetWorldDirection() { return worldDirection; }
    inline static void SetWorldDirection(glm::vec3 direction) { worldDirection = direction; }

    // controls whether the camera is being controlled by someone
    inline static bool HasControl() { return takeControl; }
    inline static bool takeControl = false;

    static const glm::mat4 &GetProjection();
    static const glm::mat4 &GetView();

    // movement
    inline static void MoveRight()
    {
        worldPosition.x += .001f; // REVIEW: This should me multiplied by the engine delta time?
        UpdateCameraPosition();
    }
    inline static void MoveLeft()
    {
        worldPosition.x -= .001f; // REVIEW: This should me multiplied by the engine delta time?
        UpdateCameraPosition();
    }
    inline static void MoveUp()
    {
        worldPosition.y -= .001f; // REVIEW: This should me multiplied by the engine delta time?
        UpdateCameraPosition();
    }
    inline static void MoveDown()
    {
        worldPosition.y += .001f; // REVIEW: This should me multiplied by the engine delta time?
        UpdateCameraPosition();
    }

private:
    // TODO: Put the projection and view matrix in a single matrix
    inline static glm::mat4 projectionMatrix{1.f};
    inline static glm::mat4 viewMatrix{1.f};

    inline static glm::vec3 worldPosition = glm::vec3(0.f);
    inline static glm::vec3 worldDirection = glm::vec3(0.f, 0.f, 1.f);
};


#endif //VULKAN_ENGINE_CAMERA_H
