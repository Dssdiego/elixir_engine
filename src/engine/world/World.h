//
// Created by Diego S. Seabra on 23/09/22.
//

#ifndef VULKAN_ENGINE_WORLD_H
#define VULKAN_ENGINE_WORLD_H

#include "../common/GameObject.h"
#include "Grid.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class World
{
public:
    static void Init();
    static void Update();
    static void Render();
    static void Shutdown();

    static void Save();
    static void Load();

    static std::vector<GameObject> *GetGameObjects();

private:
    // FIXME: Game objects should come from scenes instead of being defined here
    //        (i.e. we can change between scenes inside the world)
    inline static std::vector<GameObject> gameObjects{};
    inline static size_t currentId = 0;

    static void CreateGrid();
    static void DestroyGrid();
};


#endif //VULKAN_ENGINE_WORLD_H
