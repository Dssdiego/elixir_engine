//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_BASESHAPE_H
#define VULKAN_ENGINE_BASESHAPE_H

#include "../VulkanDevice.h"
#include "Vertex.h"
#include <vector>

class BaseShape
{
public:
    virtual void CreateShape();
    virtual void Destroy();

protected:
    std::vector<Vertex> vertices = {};

private:
    void CreateVertexBuffers();

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    uint32_t vertexCount;
};


#endif //VULKAN_ENGINE_BASESHAPE_H
