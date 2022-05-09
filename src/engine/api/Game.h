//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef VULKAN_ENGINE_GAME_H
#define VULKAN_ENGINE_GAME_H

#include <string>
#include "../common/structs.h"

class CGame
{
public:
    void Init(SEngineConfig* pConfig); // initializes everything in the engine
    void Run();  // main loop
    void Draw(); // draw loop
    void Cleanup(); // shuts down the engine

private:
    int mFrameNumber = 0;
};


#endif //VULKAN_ENGINE_GAME_H
