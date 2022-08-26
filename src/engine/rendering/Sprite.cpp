//
// Created by Diego S. Seabra on 23/08/22.
//

#include "Sprite.h"

//
// Constructor/Destructor
//

Sprite::Sprite()
{
    transform.position[0] = -0.5f;
    transform.position[1] = -0.5f;
    transform.position[2] = 0.0f;
    transform.scale[0] = 0.5f;
    transform.scale[1] = 0.5f;
    transform.rotation = 0.0f;
    shape = ShapeType::Quad;
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    color[3] = 1.0f;

    texture.Create("assets/textures/statue.jpg");
}

Sprite::~Sprite()
{
    texture.Destroy();
}
