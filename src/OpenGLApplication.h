//
// Created by Diego S. Seabra on 05/02/22.
//

#ifndef VULKAN_ENGINE_OPENGLAPPLICATION_H
#define VULKAN_ENGINE_OPENGLAPPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class OpenGLApplication
{
public:
    void run()
    {
        initWindow();
        initGlad();
        initOpenGL();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;

    void initWindow()
    {
        glfwInit();
#ifdef WIN32
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(WIDTH, HEIGHT, "Game Engine (OpenGL)", nullptr, nullptr);
        if (window == nullptr)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);
    }

    void initGlad()
    {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize glad");
        }
    }

    void initOpenGL()
    {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << " (" << glGetString(GL_VENDOR) << ")" << std::endl;
        glViewport(0,0, WIDTH, HEIGHT);
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

#endif //VULKAN_ENGINE_OPENGLAPPLICATION_H
