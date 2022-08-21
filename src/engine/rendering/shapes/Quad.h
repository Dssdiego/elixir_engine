//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_QUAD_H
#define VULKAN_ENGINE_QUAD_H

#include "Shape.h"

class Quad : public Shape
{
public:
    Quad();

    void CreateShape() override;
    void Draw() override;
    void Destroy() override;
};


#endif //VULKAN_ENGINE_QUAD_H
