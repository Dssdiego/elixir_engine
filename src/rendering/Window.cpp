//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Window.h"
#include "../profiling/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

SWindowImpl* mImplementation = nullptr;

SWindowImpl::SWindowImpl(SEngineConfig* pConfig)
{
    window = nullptr;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell GLFW to not create a OpenGL context

    // TODO: Pass engine config here
    window = glfwCreateWindow(pConfig->windowSize.width, pConfig->windowSize.height, pConfig->gameTitle.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
//    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
//    glfwSetWindowSizeLimits(window, 480, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);
//    glfwSetKeyCallback(window, keyCallback);

    mWidth = pConfig->windowSize.width;
    mHeight = pConfig->windowSize.height;

    loadIcon();
}

SWindowImpl::~SWindowImpl()
{
    glfwDestroyWindow(mImplementation->window);
    glfwTerminate();
}

void SWindowImpl::loadIcon()
{
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("../../assets/icons/icon.png", &icons[0].width, &icons[0].height, nullptr, STBI_rgb_alpha);

    if (!icons[0].pixels)
    {
        throw std::runtime_error("failed to load icon image");
    }

    glfwSetWindowIcon(window, 1, icons);

    stbi_image_free(icons[0].pixels);
}

bool CWindow::ShouldCloseWindow()
{
    return glfwWindowShouldClose(mImplementation->window);
}

void CWindow::Init(SEngineConfig* pConfig)
{
    CLogger::Info("Initializing window");
    mImplementation = new SWindowImpl(pConfig);
}

void CWindow::Update()
{
    glfwPollEvents();
}

void CWindow::Shutdown()
{
    CLogger::Info("Shutting down window");
    delete mImplementation;
}

SWindowSize CWindow::GetSize()
{
    return SWindowSize
            {
                mImplementation->mWidth,
                mImplementation->mHeight
            };
}

GLFWwindow *CWindow::GetWindow()
{
    return mImplementation->window;
}



