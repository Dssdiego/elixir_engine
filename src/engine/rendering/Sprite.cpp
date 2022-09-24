//
// Created by Diego S. Seabra on 23/08/22.
//

#include "Sprite.h"
#include "SpriteAtlas.h"

//
// Constructor/Destructor
//

Sprite::Sprite() : line(0), column(0)
{ }

Sprite::Sprite(uint32_t line, uint32_t column) : line(line), column(column)
{
    SetTile();
}

Sprite::~Sprite()
{
}

void Sprite::SetTile()
{
    auto uv = SpriteAtlas::GetUVCoordinate(line, column);
    shape.ReplaceUVs(uv.x, uv.y, uv.z, uv.w); // FIXME: Refactor me, not very good legibility!
}
