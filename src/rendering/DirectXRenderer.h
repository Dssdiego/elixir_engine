//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef VULKAN_ENGINE_DIRECTXRENDERER_H
#define VULKAN_ENGINE_DIRECTXRENDERER_H

#include <d3d12.h>
#include <dxgi1_4.h>

#include <stdexcept>
#include <iostream>

#pragma comment(lib, "dxgi.lib")

struct CDirectXRendererImpl
{
    CDirectXRendererImpl();
    ~CDirectXRendererImpl();

    IDXGIFactory4* mFactory;
    IDXGIAdapter1* mAdapter;
    ID3D12Device* mDevice;
    ID3D12CommandQueue* mCommandQueue;
    ID3D12CommandAllocator* mCommandAllocator;

    UINT mDxgiFactoryFlags = 0;

    void CreateFactory();
};

class CDirectXRenderer
{
public:
    static void Init();
    static void Draw();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_DIRECTXRENDERER_H
