//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef VULKAN_ENGINE_STRUCTS_H
#define VULKAN_ENGINE_STRUCTS_H

#include <string>

struct SWindowSize
{
    uint32_t width;
    uint32_t height;
};

struct SEngineConfig
{
    std::string gameTitle;
    SWindowSize windowSize;
    SWindowSize minWindowSize;
};

#endif //VULKAN_ENGINE_STRUCTS_H
