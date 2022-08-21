//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Shape.h"

//
// Public/External
//

void Shape::CreateShape()
{
    CreateVertexBuffers();
}

void Shape::Draw()
{
    Bind();
    vkCmdDraw(EngineRenderer::GetCurrentCommandBuffer(), vertexCount, 1, 0, 0);
}

void Shape::Destroy()
{
    vkDestroyBuffer(VulkanDevice::GetDevice(), vertexBuffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), vertexBufferMemory, nullptr);
}

//
// Private
//

void Shape::CreateVertexBuffers()
{
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3 so that we can draw triangles ;)");

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    VulkanDevice::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory
    );

    void *data;
    vkMapMemory(VulkanDevice::GetDevice(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(VulkanDevice::GetDevice(), vertexBufferMemory);
}

void Shape::Bind()
{
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(EngineRenderer::GetCurrentCommandBuffer(), 0, 1, buffers, offsets);
}
