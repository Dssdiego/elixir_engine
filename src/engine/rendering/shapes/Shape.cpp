//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Shape.h"

//
// Public/External
//

Shape::Shape(ShapeType type)
{
    switch (type)
    {
        case ShapeType::Triangle:
            DefineTriangle();
            break;

        case ShapeType::Quad:
            DefineQuad();
            break;

        case ShapeType::Circle:
            DefineCircle();
            break;
    }

    MakeShape();
}

void Shape::MakeShape()
{
    CreateVertexBuffer();
    CreateIndexBuffer();
}

void Shape::Draw()
{
    Bind();
    vkCmdDrawIndexed(EngineRenderer::GetCurrentCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Shape::Destroy()
{
    // vertex buffer
    vkDestroyBuffer(VulkanDevice::GetDevice(), vertexBuffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), vertexBufferMemory, nullptr);

    // index buffer
    vkDestroyBuffer(VulkanDevice::GetDevice(), indexBuffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), indexBufferMemory, nullptr);
}

//
// Private
//

void Shape::CreateVertexBuffer()
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

void Shape::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanDevice::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanDevice::GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(VulkanDevice::GetDevice(), stagingBufferMemory);

    VulkanDevice::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                             VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer,
                 indexBufferMemory);

    VulkanDevice::CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(VulkanDevice::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), stagingBufferMemory, nullptr);
}

void Shape::Bind()
{
    // bind the vertex buffer
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(EngineRenderer::GetCurrentCommandBuffer(), 0, 1, buffers, offsets);

    // bind the index buffer
    vkCmdBindIndexBuffer(EngineRenderer::GetCurrentCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

//
// Shape Definition
//

void Shape::DefineTriangle()
{
    vertices = {
            {{0.0f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f, 0.f}, {0.f, 0.f}},
            {{0.5f, 0.5f, 0.f}, {0.0f, 1.0f, 0.0f, 0.f}, {0.f, 0.f}},
            {{-0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f, 0.f}, {0.f, 0.f}}
    };

    indices = {
            0, 1, 2
    };
}

void Shape::DefineQuad()
{
    // position (3) | color (4) | uv (2)
    vertices = {
            {{-0.5f, -0.5f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f}},
            {{0.5f, -0.5f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 0.f}},
            {{0.5f, 0.5f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 1.f}},
            {{-0.5f, 0.5f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 1.f}}
    };

    indices = {
            0, 1, 2, 2, 3, 0
    };
}

// TODO: Reimplement circle now that we have to consider UV coordinates
void Shape::DefineCircle()
{
//    int numSides = 64;
//
//    std::vector<Vertex> uniqueVertices{};
//
//    for (int i = 0; i < numSides; i++) {
//        float angle = i * glm::two_pi<float>() / numSides;
//        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle), 0.f}});
//    }
//
//    uniqueVertices.push_back({}); // adding center vertex at (0,0)
//
//    for (int i = 0; i < numSides; i++) {
//        vertices.push_back(uniqueVertices[i]);
//        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
//        vertices.push_back(uniqueVertices[numSides]);
//    }
}
