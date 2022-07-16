//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef VULKAN_ENGINE_RENDERER_H
#define VULKAN_ENGINE_RENDERER_H

enum GraphicsBackend
{
    DIRECTX,
    VULKAN,
    OPENGL
};

class Renderer
{
public:
    static void Init(GraphicsBackend backend);
    static void Draw();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_RENDERER_H
