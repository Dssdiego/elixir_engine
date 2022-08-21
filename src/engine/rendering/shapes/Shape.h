//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_SHAPE_H
#define VULKAN_ENGINE_SHAPE_H

#include "../VulkanDevice.h"
#include "../EngineRenderer.h"
#include "Vertex.h"
#include <vector>

class Shape
{
public:
    virtual void CreateShape();
    virtual void Draw();
    virtual void Destroy();

protected:
    std::vector<Vertex> vertices;

private:
    void CreateVertexBuffers();
    void Bind();

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    uint32_t vertexCount;
};


#endif //VULKAN_ENGINE_SHAPE_H
