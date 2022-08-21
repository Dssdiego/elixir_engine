//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef VULKAN_ENGINE_TEXTUREDQUAD_H
#define VULKAN_ENGINE_TEXTUREDQUAD_H

#include "Shape.h"
#include "../Texture.h"

class TexturedQuad : public Shape
{
public:
    TexturedQuad(const std::string &imagePath);

    void CreateShape() override;
    void Draw() override;
    void Destroy() override;

private:
    Texture texture{nullptr};

    void CreateTexture(const std::string &imagePath);
};

#endif //VULKAN_ENGINE_TEXTUREDQUAD_H
