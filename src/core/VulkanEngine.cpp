//
// Created by Diego S. Seabra on 08/03/22.
//

#include "VulkanEngine.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"

void CVulkanEngine::Init(EngineConfig* pConfig)
{
    mWindowTitle = pConfig->title;
    mWindowSize = pConfig->windowSize;
    mMinWindowSize = pConfig->minWindowSize;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell GLFW to not create a OpenGL context

    mWindow = glfwCreateWindow(mWindowSize.width, mWindowSize.height, mWindowTitle.c_str(), nullptr, nullptr);
    glfwSetWindowSizeLimits(mWindow, mMinWindowSize.width, mMinWindowSize.height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    // TODO: custom cursor, window resize, input key callback
//    glfwSetWindowUserPointer(window, this);
//    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
//    glfwSetKeyCallback(window, keyCallback);

    // init engine systems
    CAudioEngine::Init();
    CInput::Init();

    // everything went fine
    mIsInitialized = true;
}

void CVulkanEngine::Run()
{
    while(!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();

        Draw();

        CAudioEngine::Update();
    }
}

void CVulkanEngine::Draw()
{

}

void CVulkanEngine::Cleanup()
{
    // destroy engine systems
    CAudioEngine::Shutdown();
    CInput::Shutdown();

    glfwDestroyWindow(mWindow);
//    glfwDestroyCursor(cursor); // TODO: Destroy cursor (when loaded)
    glfwTerminate();
}


