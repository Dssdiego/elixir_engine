//
// Created by Diego S. Seabra on 13/08/22.
//

#ifndef VULKAN_ENGINE_COLOR_H
#define VULKAN_ENGINE_COLOR_H

#include <cstdint>
//#include <string>

class Color
{
public:
    // Constructor
    Color(float r, float g, float b, float a);
//    Color(std::string *hex); // TODO: Implement hex transform in rgba

    // Helpers
    static VkClearColorValue GetAsVulkanClear(Color color);

    // Definitions
    static const Color Transparent;
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Pink;
    static const Color Cyan;
    static const Color Background;

    float r, g, b, a;
};


#endif //VULKAN_ENGINE_COLOR_H
