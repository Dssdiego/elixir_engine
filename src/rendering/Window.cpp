//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Window.h"

SWindowImpl* mImplementation = nullptr;

SWindowImpl::SWindowImpl(SEngineConfig* pConfig)
{
    window = nullptr;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell GLFW to not create a OpenGL context

    // TODO: Pass engine config here
    window = glfwCreateWindow(pConfig->windowSize.width, pConfig->windowSize.height, pConfig->gameTitle.c_str(), nullptr, nullptr);
//    glfwSetWindowUserPointer(window, this);
//    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
//    glfwSetWindowSizeLimits(window, 480, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);
//    glfwSetKeyCallback(window, keyCallback);
}

SWindowImpl::~SWindowImpl()
{
    glfwDestroyWindow(mImplementation->window);
    glfwTerminate();
}

bool CWindow::ShouldCloseWindow()
{
    return glfwWindowShouldClose(mImplementation->window);
}

void CWindow::Init(SEngineConfig* pConfig)
{
    mImplementation = new SWindowImpl(pConfig);
}

void CWindow::Update()
{
    glfwPollEvents();
}

void CWindow::Shutdown()
{
    delete mImplementation;
}



