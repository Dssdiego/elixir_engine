//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_TRIANGLE_H
#define VULKAN_ENGINE_TRIANGLE_H

#include "BaseShape.h"

class Triangle : public BaseShape
{
public:
    void CreateShape() override;

    void Destroy() override;

    void Create();
    void Draw() override;
};


#endif //VULKAN_ENGINE_TRIANGLE_H
