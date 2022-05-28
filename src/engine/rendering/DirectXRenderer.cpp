//
// Created by Diego S. Seabra on 14/03/22.
//

#ifdef _WIN32
#include "DirectXRenderer.h"
#include "../profiling/Logger.h"
#include "Window.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one

CDirectXRendererImpl* mImplementation = nullptr;

void CDirectXRenderer::Init()
{
    mImplementation = new CDirectXRendererImpl;
}

void CDirectXRenderer::Shutdown()
{
    delete mImplementation;
}

CDirectXRendererImpl::CDirectXRendererImpl()
{
#ifndef NDEBUG
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&mDebugController))))
    {
        mDebugController->EnableDebugLayer();
    }
#endif

    CreateFactory();
    CreateAdapter();
    CreateDevice();
    CreateCommandQueue();
    CreateCommandAllocator();
    CreateFencePrimitive();
//    CreateBarrierPrimitive();
//    CreateSwapchain();

#ifndef NDEBUG
    CreateDebugDevice();
#endif
}

CDirectXRendererImpl::~CDirectXRendererImpl()
{
    // NOTE: Using smart pointers will deprecate this code, as we
    //  won't need to worry about releasing memory
    mFactory->Release();
    mAdapter->Release();
    mDevice->Release();
    mCommandQueue->Release();
    mCommandAllocator->Release();
    mFence->Release();
//    mCommandList->Release(); // barrier code
//    mSwapchain->Release();
}

#ifndef NDEBUG
void CDirectXRendererImpl::CreateDebugDevice()
{
    if (FAILED((mDevice->QueryInterface(&mDebugDevice))))
    {
        throw std::runtime_error("DIRECTX::Failed to create debug device");
    }
}
#endif

// Factories are the entry point to the DirectX 12 API,
// and will allow us to find adapters that we can use to execute DirectX 12 commands.
void CDirectXRendererImpl::CreateFactory()
{
    // debug controller to track errors
#ifndef NDEBUG
    ID3D12Debug* dc;
    if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&dc))))
    {
        throw std::runtime_error("DIRECTX::Failed to create debug interface");
    }

    if (FAILED(dc->QueryInterface(IID_PPV_ARGS(&mDebugController))))
    {
        throw std::runtime_error("DIRECTX::Failed to query debug interface from debug controller");
    }

    mDxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    dc->Release();
    dc = nullptr;
#endif

    CLogger::Info("DIRECTX::CreateFactory");
    if FAILED(CreateDXGIFactory2(mDxgiFactoryFlags, IID_PPV_ARGS(&mFactory)))
    {
        throw std::runtime_error("DIRECTX::Failed to create factory");
    }
}

// An Adapter provides information on the physical properties of a given DirectX device.
// We can query the current GPU, how much memory it has, etc.
void CDirectXRendererImpl::CreateAdapter()
{
    CLogger::Info("DIRECTX::CreateAdapter");
    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != mFactory->EnumAdapters1(adapterIndex, &mAdapter); ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        mAdapter->GetDesc1(&desc);

        // Don't select the "Basic Render Driver" adapter
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        // Check if the adapter supports Direct3D 12, and use that for the rest of the application
        if (SUCCEEDED(D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
            break;

        // Else we won't use this iteration's adapter, so release it
        mAdapter->Release();
    }
}

// A Device is our primary entry point to the DirectX 12 API, giving us access to the inner parts of the API.
// This is key to accessing important data structures and functions such as pipelines, shader blobs, render state,
// resource barriers, etc.
void CDirectXRendererImpl::CreateDevice()
{
    CLogger::Info("DIRECTX::CreateDevice");
    if (FAILED(D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mDevice))))
    {
        throw std::runtime_error("DIRECTX::Failed to create a Direct3D 12.0 device");
    }
}

// A Command Queue allows us to submit groups of draw calls, known as command lists, together to execute in order,
// thus allowing a GPU to stay busy and optimize its execution speed.
void CDirectXRendererImpl::CreateCommandQueue()
{
    CLogger::Info("DIRECTX::CreateCommandQueue");
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    if (FAILED(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue))))
    {
        throw std::runtime_error("DIRECTX::Failed to create command queue");
    }
}

// A Command Allocator allows us to create command lists where we can define the functions we want
// the GPU to execute.
void CDirectXRendererImpl::CreateCommandAllocator()
{
    CLogger::Info("DIRECTX::CreateCommandAllocator");
    if (FAILED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator))))
    {
        throw std::runtime_error("DIRECTX::Failed to create command allocator");
    }
}

// A Fence lets our program know when certain tasks have been executed by the GPU, be it uploads to GPU exclusive memory,
// or when we've finished presenting to the screen
void CDirectXRendererImpl::CreateFencePrimitive()
{
    CLogger::Info("DIRECTX::CreateFencePrimitive");
    if (FAILED(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence))))
    {
        throw std::runtime_error("DIRECTX::Failed to create fence primitive");
    }
}

// A Barrier lets the driver know how a resource should be used in upcoming commands. This can be useful if say,
// we're writing to a texture, and we want to copy that texture to another texture (such as the swapchain's render attachment)
void CDirectXRendererImpl::CreateBarrierPrimitive()
{
    CLogger::Info("DIRECTX::CreateBarrierPrimitive");

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = nullptr; // REVIEW: Pass a resource here?
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mCommandList->ResourceBarrier(1, &barrier);
}

// Swapchains handle swapping and allocating back buffers to display
// what we're rendering to a given window
void CDirectXRendererImpl::CreateSwapchain()
{
    CLogger::Info("DIRECTX::CreateSwapchain");

    mSurfaceSize.left = 0;
    mSurfaceSize.top = 0;
    mSurfaceSize.right = static_cast<LONG>(CWindow::GetSize().width);
    mSurfaceSize.bottom = static_cast<LONG>(CWindow::GetSize().height);

    mViewport.TopLeftX = 0.0f;
    mViewport.TopLeftY = 0.0f;
    mViewport.Width = static_cast<float>(CWindow::GetSize().width);
    mViewport.Height = static_cast<float>(CWindow::GetSize().height);
    mViewport.MinDepth = 0.1f;
    mViewport.MaxDepth = 1000.0f;

    // REVIEW
//    if (mSwapchain != nullptr)
//    {
//        // Create render target attachments from swapchain
//    } else {
//
//    }

    // NOTE: Using DXGI_SWAP_CHAIN_DESC instead o DXGI_SWAP_CHAIN_DESC1 here
    DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
    swapchainDesc.BufferCount = mBackbufferCount;
    swapchainDesc.BufferDesc.Width = 800; // FIXME
    swapchainDesc.BufferDesc.Height = 600; // FIXME
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.OutputWindow = nullptr;

    IDXGISwapChain* newSwapchain;
//    mFactory->CreateSwapChainForHwnd() // TODO: Try this
    if (FAILED(mFactory->CreateSwapChain(mCommandQueue, &swapchainDesc, &newSwapchain)))
    {
        throw std::runtime_error("DIRECTX::Failed to create swapchain");
    }

//    if SUCCEEDED(mSwapchain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&newSwapchain))
//    {
//        mSwapchain = (IDXGISwapChain3*) newSwapchain;
//    }

//    mFrameIndex = mSwapchain->GetCurrentBackBufferIndex();

    // Describe and create a render target view (RTV) descriptor heap
}

#endif
