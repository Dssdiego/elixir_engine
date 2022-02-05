//
// Created by Diego S. Seabra on 05/02/22.
//

// TODO: Raw Directx 12 Tutorial :: https://alain.xyz/blog/raw-directx12

#ifndef VULKAN_ENGINE_DIRECTXAPPLICATION_H
#define VULKAN_ENGINE_DIRECTXAPPLICATION_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <d3d12.h>
#include <dxgi1_4.h>

#pragma comment(lib, "dxgi.lib")

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class DirectXApplication
{
public:
    void run()
    {
        initWindow();
        initDirectX();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    UINT dxgiFactoryFlags = 0;
    IDXGIFactory4* factory;
    IDXGIAdapter1* adapter;
    ID3D12Device* device;
    ID3D12CommandQueue* commandQueue;
    ID3D12CommandAllocator* commandAllocator;

    void initWindow()
    {

    }

    // Factories are the entry point to the DirectX 12 API,
    // and will allow you to find adapters that you can use to execute DirectX 12 commands.
    void createFactory()
    {
        // REVIEW:: Create a debug controller to track errors?
//        HRESULT result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
        if FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)))
        {
            throw std::runtime_error("Failed to create factory");
        }
    }

    // An Adapter provides information on the physical properties of a given DirectX device.
    // You can query your current GPU, how much memory it has, etc.
    void createAdapter()
    {
        for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            // Don't select the "Basic Render Driver" adapter
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            // Check if the adapter supports Direct3D 12, and use that for the rest of the application
            if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
                break;

            // Else we won't use this iteration's adapter, so release it
            adapter->Release();
        }
    }

    // A Device is your primary entry point to the DirectX 12 API, giving you access to the inner parts of the API.
    // This is key to accessing important data structures and functions such as pipelines, shader blobs, render state,
    // resource barriers, etc.
    void createDevice()
    {
        if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device))))
        {
            throw std::runtime_error("Failed to create a Direct3D 12.0 device");
        }
    }

    // A Command Queue allows you to submit groups of draw calls, known as command lists, together to execute in order,
    // thus allowing a GPU to stay busy and optimize its execution speed.
    void createCommandQueue()
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue))))
        {
            throw std::runtime_error("Failed to create command queue");
        }
    }

    // A Command Allocator allows you to create command lists where you can define the functions you want
    // the GPU to execute.
    void createCommandAllocator()
    {
        if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator))))
        {
            throw std::runtime_error("Failed to create command allocator");
        }
    }

    // DirectX 12 features a number of synchronization primitives that can help the driver know how resources will
    // be used in the future, know when tasks have been completed by the GPU, etc.
    void createSynchronizationPrimitives()
    {
        // TODO: Continue implementation
        // Fence
        // A Fence lets your program know when certain tasks have been executed by the GPU, be it uploads to GPU
        // exclusive memory, or when you've finished presenting to the screen.
//        if (FAILED())


        // Barrier
        // A Barrier lets the driver know how a resource should be used in upcoming commands. This can be useful if say,
        // you're writing to a texture, and you want to copy that texture to another texture (such as the swapchain's render attachment).
    }

    void createSwapChain()
    {

    }

    void initDirectX()
    {
        createFactory();
        createAdapter();
        createDevice();
        createCommandQueue();
        createCommandAllocator();
        createSynchronizationPrimitives();
        createSwapChain();
    }

    void mainLoop()
    {

    }

    void cleanup()
    {
        factory->Release();
        adapter->Release();
        device->Release();
        commandQueue->Release();
        commandAllocator->Release();
    }
};

#endif //VULKAN_ENGINE_DIRECTXAPPLICATION_H
