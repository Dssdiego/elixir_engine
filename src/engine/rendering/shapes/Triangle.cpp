//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Triangle.h"

void Triangle::Create()
{
    // TODO: Continue here
    vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    CreateShape();
}
