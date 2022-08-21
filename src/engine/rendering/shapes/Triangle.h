//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_TRIANGLE_H
#define VULKAN_ENGINE_TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle();

    void CreateShape() override;
    void Draw() override;
    void Destroy() override;
};


#endif //VULKAN_ENGINE_TRIANGLE_H
