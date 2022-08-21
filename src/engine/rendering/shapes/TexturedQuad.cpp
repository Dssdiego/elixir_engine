//
// Created by Diego S. Seabra on 20/08/22.
//

#include "TexturedQuad.h"

TexturedQuad::TexturedQuad()
{
    CreateShape();
}

void TexturedQuad::CreateShape()
{
    vertices = {
            {{-0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}},

            {{-0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}},
    };

    // TODO: Use indices here so that we don't draw duplicated vertices! ;)

    Shape::CreateShape();
}

void TexturedQuad::Draw()
{
    Shape::Draw();
}

void TexturedQuad::Destroy()
{
    Shape::Destroy();
}
