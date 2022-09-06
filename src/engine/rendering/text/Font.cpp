//
// Created by Diego S. Seabra on 03/09/22.
//

#include "Font.h"

void Font::LoadFont()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        Logger::Error("Couldn't load the FreeType library!", "");
    else
        Logger::Debug("Initialized FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/Silver.ttf", 0, &face))
        Logger::Error("Failed to load font!", "");
    else
        Logger::Debug("Loaded Silver font");

    // setting the pixel font size to 48px
    FT_Set_Pixel_Sizes(face, 0 ,48); // setting the width to 0 lets the face dynamically calculate the width based on the given height
}
