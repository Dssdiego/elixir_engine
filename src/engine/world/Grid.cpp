//
// Created by Diego S. Seabra on 27/09/22.
//

#include "Grid.h"

void Grid::Create(uint32_t lines, uint32_t columns)
{
    for (int i = 0; i < lines; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            std::stringstream ss;
            ss << "(" << i << "," << j << ")";
            auto gridTile = GridTile{
                ss.str(),
                glm::vec3((float) i * tileSize, (float) j * tileSize, 0.f),
                glm::vec2(i,j)
            };
            tiles.emplace_back(gridTile);
        }
    }
}
