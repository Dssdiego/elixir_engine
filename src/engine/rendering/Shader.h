//
// Created by Diego S. Seabra on 18/05/22.
//

#ifndef VULKAN_ENGINE_SHADER_H
#define VULKAN_ENGINE_SHADER_H

#include <string>
#include <vector>
#include <fstream>
#include <vulkan/vulkan.h>

class Shader
{
public:
    static std::vector<char> ReadFile(const std::string& filename);
};


#endif //VULKAN_ENGINE_SHADER_H
