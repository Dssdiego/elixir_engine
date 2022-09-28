//
// Created by Diego S. Seabra on 27/09/22.
//

#include "Grid.h"

Grid::Grid(uint32_t lines, uint32_t columns) : lines(lines), columns(columns)
{
    Create();
}

void Grid::Create()
{
    for (int i = 0; i < lines; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            std::stringstream ss;
            ss << "(" << i << "," << j << ")";
            auto gridTile = GridTile{
                ss.str(),
                glm::vec3((float) i * tileSize, (float) j * tileSize, 0.f)
            };
            tiles.emplace_back(gridTile);
        }
    }
}
