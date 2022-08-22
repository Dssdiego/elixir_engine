//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_SHAPE_H
#define VULKAN_ENGINE_SHAPE_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../VulkanDevice.h"
#include "../EngineRenderer.h"
#include "Vertex.h"
#include <vector>

enum ShapeType {
    Triangle,
    Circle,
    Square
};

class Shape
{
public:
    Shape(ShapeType type);

    virtual void Draw();
    virtual void Destroy();

protected:
    std::vector<Vertex> vertices;

private:
    void DefineTriangle();
    void DefineCircle();
    void DefineSquare();

    void MakeShape();
    void CreateVertexBuffers();
    void Bind();

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    uint32_t vertexCount;
};


#endif //VULKAN_ENGINE_SHAPE_H
