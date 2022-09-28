//
// Created by Diego S. Seabra on 27/09/22.
//

#ifndef VULKAN_ENGINE_GRID_H
#define VULKAN_ENGINE_GRID_H

#include <iostream>
#include <glm/vec3.hpp>
#include <vector>
#include <sstream>
//#include "../common/Color.h"

struct GridTile
{
    std::string name;
    glm::vec3 worldPosition;
};

class Grid
{
public:
    Grid(uint32_t lines, uint32_t columns);

    void Create();
    inline std::vector<GridTile> GetTiles() { return tiles; }

    inline float TileSize() const { return tileSize; }
    inline float Rotation() const { return rotation; }
//    inline Color Color() const { return color; }

private:
    uint32_t lines = 0;
    uint32_t columns = 0;

    std::vector<GridTile> tiles{};

//    class Color color = Color::Green;
    float tileSize = 0.15f;
    float rotation = 0.f;
};


#endif //VULKAN_ENGINE_GRID_H
