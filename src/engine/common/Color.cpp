//
// Created by Diego S. Seabra on 13/08/22.
//

#include <vulkan/vulkan_core.h>
#include "Color.h"

// Constructor
Color::Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}

// Definitions
const Color Color::Transparent  = {0.0f,0.0f,0.0f,0.0f};
const Color Color::Black        = {0.0f,0.0f,0.0f,1.0f};
const Color Color::White        = {1.0f,1.0f,1.0f,1.0f};
const Color Color::Red          = {1.0f,0.0f,0.0f,1.0f};
const Color Color::Green        = {0.0f,1.0f,0.0f,1.0f};
const Color Color::Blue         = {0.0f,0.0f,1.0f,1.0f};
const Color Color::Yellow       = {1.0f,1.0f,0.0f,1.0f};
const Color Color::Pink         = {1.0f,0.0f,1.0f,1.0f};
const Color Color::Cyan         = {0.0f,1.0f,1.0f,1.0f};
const Color Color::Background   = {0.47f, 0.42f, 0.96f, 1.0f};

VkClearColorValue Color::GetAsVulkanClear(Color color)
{
    VkClearColorValue result;
    result.float32[0] = color.r;
    result.float32[1] = color.g;
    result.float32[2] = color.b;
    result.float32[3] = color.a;
    return result;
}
