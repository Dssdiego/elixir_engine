//
// Created by Diego S. Seabra on 18/05/22.
//

#ifndef VULKAN_ENGINE_VERTEX_H
#define VULKAN_ENGINE_VERTEX_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.h>
#include <array>

struct Vertex {
    glm::vec2 pos;
    glm::vec4 color;
//    glm::vec2 texCoord;
    // NOTE: Disabling texture coordinates for now
    // TODO: Re-enable texture coordinates

    // vertex binding describes at which rate to load data from memory throughout the vertices
    //   it specifies the number of bytes between data entries and whether
    //   to move to the next data entry after each vertex or after each instance
    static VkVertexInputBindingDescription GetBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0; // index of the binding in the array of bindings
        bindingDescription.stride = sizeof(Vertex); // number of bytes from one entry to the next
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // move to the next data entry after each vertex

        return bindingDescription;
    }

    // an attribute description struct describes how to extract a vertex attribute from a chunk of vertex data
    //  originating from a binding description
    // NOTE: Disabling texture coordinates for now
    // TODO: Re-enable texture coordinates
    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[0].location = 0; // location directive of the input in the vertex shader
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // inPosition is a vec3 (same enumeration as color formats)
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[1].location = 1; // location directive of the input in the vertex shader
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT; // color is a vec4
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        // NOTE: Disabling texture coordinates for now
        // TODO: Re-enable texture coordinates
//        attributeDescriptions[2].binding = 0; // which binding the per-vertex data comes
//        attributeDescriptions[2].location = 2; // location directive of the input in the vertex shader
//        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT; // texCoord is a vec2
//        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};


#endif //VULKAN_ENGINE_VERTEX_H
