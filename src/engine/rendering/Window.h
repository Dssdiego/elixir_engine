//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef VULKAN_ENGINE_WINDOW_H
#define VULKAN_ENGINE_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include "../common/structs.h"

struct WindowImpl
{
    WindowImpl(EngineConfig* pConfig);
    ~WindowImpl();

    GLFWwindow* window;

    uint32_t mWidth, mHeight;
    std::string title;

    void loadIcon();
};

class Window
{
public:
    // lifecycle methods
    static void Init(EngineConfig* pConfig);
    static void Update();
    static void Shutdown();

    // vulkan stuff
    static std::vector<const char *> GetRequiredExtensions();

    // methods
    static bool ShouldCloseWindow();
    static WindowSize GetSize();
    static GLFWwindow* GetWindow();
    static void UpdateFPSInTitle(double fps);
    static double GetTime();

private:
    void LoadCursor();
    void Resize();

};

#endif //VULKAN_ENGINE_WINDOW_H
