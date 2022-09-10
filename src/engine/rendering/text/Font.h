//
// Created by Diego S. Seabra on 03/09/22.
//

#ifndef VULKAN_ENGINE_FONT_H
#define VULKAN_ENGINE_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <fstream>

#include "../../profiling/Logger.h"

#ifdef _WIN32

class Font
{
public:
    static void LoadFont();
};

#endif


#endif //VULKAN_ENGINE_FONT_H