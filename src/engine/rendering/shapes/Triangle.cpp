//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Triangle.h"

Triangle::Triangle()
{
    CreateShape();
}

void Triangle::CreateShape()
{
    vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    Shape::CreateShape();
}

void Triangle::Draw()
{
    Shape::Draw();
}

void Triangle::Destroy()
{
    Shape::Destroy();
}
