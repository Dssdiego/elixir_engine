//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Triangle.h"

void Triangle::CreateShape()
{
    auto colorWhite = glm::vec3(1.0f, 1.0f, 1.0f);

    vertices = {
            {{-0.5f, -0.5f, 0.0f}, colorWhite, { 0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, colorWhite, { 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, colorWhite, { 1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, colorWhite, { 0.0f, 1.0f}},
    };

    BaseShape::CreateShape();
}

void Triangle::Draw()
{
    BaseShape::Draw();
}

void Triangle::Destroy()
{
    BaseShape::Destroy();
}
