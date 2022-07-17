//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Window.h"
#include "../profiling/Logger.h"
#include "../common/Config.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Tracy.hpp>

WindowImpl* mWindowImpl = nullptr;

WindowImpl::WindowImpl(EngineConfig* pConfig)
{
    window = nullptr;

    Logger::Debug("initializing glfw");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell GLFW to not create a OpenGL context (because we might use Vulkan/DirectX)

    // TODO: Pass engine config here
    Logger::Debug("creating window");
    window = glfwCreateWindow(pConfig->windowSize.width, pConfig->windowSize.height, pConfig->gameTitle.c_str(), nullptr, nullptr);

    if (window == nullptr)
        Logger::Error("failed to create window with GLFW", "window == nullptr");
    else
        Logger::Info("glfw window created successfully");

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowPos(window, Config::GetWindowPositionX(), Config::GetWindowPositionY());
//    glfwSetFramebufferSizeCallback(window, WindowResized);
//    glfwSetWindowSizeLimits(window, 480, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);
//    glfwSetKeyCallback(window, keyCallback);

    mWidth = pConfig->windowSize.width;
    mHeight = pConfig->windowSize.height;

    loadIcon();
}

WindowImpl::~WindowImpl()
{
    glfwDestroyWindow(mWindowImpl->window);
    glfwTerminate();
}

void WindowImpl::loadIcon()
{
    Logger::Debug("loading icon");

    GLFWimage icons[1];
    icons[0].pixels = stbi_load("assets/icons/icon.png", &icons[0].width, &icons[0].height, nullptr, STBI_rgb_alpha);

    if (!icons[0].pixels)
    {
        throw std::runtime_error("failed to load icon image");
    }

    glfwSetWindowIcon(window, 1, icons);

    stbi_image_free(icons[0].pixels);
}

bool Window::ShouldCloseWindow()
{
    return glfwWindowShouldClose(mWindowImpl->window);
}

void Window::Init(EngineConfig* pConfig)
{
    Logger::Info("Initializing window");
    mWindowImpl = new WindowImpl(pConfig);
}

void Window::Update()
{
    ZoneScopedC(0x2ecc71);
    glfwPollEvents();
}

void Window::Shutdown()
{
    Logger::Info("Shutting down window");
    delete mWindowImpl;
}

WindowSize Window::GetSize()
{
    return WindowSize
            {
        mWindowImpl->mWidth,
        mWindowImpl->mHeight
            };
}

GLFWwindow *Window::GetWindow()
{
    return mWindowImpl->window;
}

std::vector<const char *> Window::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::cout << "extensions count: " << glfwExtensionCount << std::endl;
    std::cout << *glfwExtensions << std::endl;

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    return extensions;
}
