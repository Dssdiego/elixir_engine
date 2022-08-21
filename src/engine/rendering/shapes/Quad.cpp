//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Quad.h"

Quad::Quad()
{
    CreateShape();
}

void Quad::CreateShape()
{
    vertices = {
            {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},

            {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    };

    // TODO: Use indices here so that we don't draw duplicated vertices! ;)

    Shape::CreateShape();
}

void Quad::Draw()
{
    Shape::Draw();
}

void Quad::Destroy()
{
    Shape::Destroy();
}
