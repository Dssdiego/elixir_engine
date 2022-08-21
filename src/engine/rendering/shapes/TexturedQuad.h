//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_TEXTUREDQUAD_H
#define VULKAN_ENGINE_TEXTUREDQUAD_H

#include "Shape.h"

class TexturedQuad : public Shape
{
public:
    TexturedQuad();

    void CreateShape() override;
    void Draw() override;
    void Destroy() override;

private:

};

#endif //VULKAN_ENGINE_TEXTUREDQUAD_H
