//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef VULKAN_ENGINE_RENDERER_H
#define VULKAN_ENGINE_RENDERER_H

enum SBackend
{
    DIRECTX,
    VULKAN,
    OPENGL
};

class CRenderer
{
public:
    static void Init(SBackend backend);
    static void Draw();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_RENDERER_H
