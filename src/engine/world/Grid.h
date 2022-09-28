//
// Created by Diego S. Seabra on 27/09/22.
//

#ifndef VULKAN_ENGINE_GRID_H
#define VULKAN_ENGINE_GRID_H

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <sstream>
//#include "../common/Color.h"

struct GridTile
{
    std::string name;
    glm::vec3 worldPosition;
    glm::vec2 tilePosition;
};

class Grid
{
public:
    static void Create(uint32_t lines, uint32_t columns);
    inline static std::vector<GridTile> GetTiles() { return tiles; }

    inline static float TileSize() { return tileSize; }
    inline static float Rotation() { return rotation; }
//    inline Color Color() const { return color; }

private:
//    inline static uint32_t lines = 0;
//    inline static uint32_t columns = 0;

    inline static std::vector<GridTile> tiles{};

//    class Color color = Color::Green;
    inline static float tileSize = 0.15f;
    inline static float rotation = 0.f;
};


#endif //VULKAN_ENGINE_GRID_H
