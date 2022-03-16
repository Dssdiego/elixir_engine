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

struct SFencePrimitive
{
    UINT frameIndex;
    HANDLE fenceEvent;
    ID3D12Fence* fence;
    UINT64 fenceValue;
};

struct CDirectXRendererImpl
{
    CDirectXRendererImpl();
    ~CDirectXRendererImpl();

    IDXGIFactory4* mFactory;
    IDXGIAdapter1* mAdapter;
    ID3D12Device* mDevice;
    ID3D12CommandQueue* mCommandQueue;
    ID3D12CommandAllocator* mCommandAllocator;
    SFencePrimitive mFencePrimitive;
    ID3D12GraphicsCommandList* mCommandList;
    IDXGISwapChain3* mSwapchain;
    D3D12_VIEWPORT mViewport;
    D3D12_RECT mSurfaceSize;

    UINT mBackbufferCount = 2;
    UINT mDxgiFactoryFlags = 0;

    void CreateFactory();
    void CreateAdapter();
    void CreateDevice();
    void CreateCommandQueue();
    void CreateCommandAllocator();
    void CreateFencePrimitive();
    void CreateBarrierPrimitive();
    void CreateSwapchain();

//#ifndef NDEBUG
    void CreateDebugDevice();
    ID3D12Debug1* mDebugController;
    ID3D12DebugDevice* mDebugDevice;
//#endif
};

class CDirectXRenderer
{
public:
    static void Init();
    static void Draw();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_DIRECTXRENDERER_H
