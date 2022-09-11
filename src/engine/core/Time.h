//
// Created by Diego S. Seabra on 11/09/22.
//

#ifndef VULKAN_ENGINE_TIME_H
#define VULKAN_ENGINE_TIME_H


#include <cstdint>

class Time
{
public:
    inline static void SetDelta(double dt) { delta = dt; }
    inline static double GetDelta() { return delta; }

private:
    inline static double delta = 0.f;
};


#endif //VULKAN_ENGINE_TIME_H
