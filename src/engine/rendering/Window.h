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
    static double GetTime();
    static void Close();

    inline static void SetFrameInfo(double _fps, uint32_t _frameNumber) {
        fps = _fps;
        frameNumber = _frameNumber;

        if (fpsHistory.size() > 50)
            fpsHistory.resize(0);
        else
            fpsHistory.push_back((float) fps);
    }
    inline static double GetFPS() { return fps; }
    inline static std::vector<float> GetFPSHistory() { return fpsHistory; }
    inline static uint32_t GetFrameNumber() { return frameNumber; }

private:
    void LoadCursor();
    void Resize();

    inline static double fps = 0.f;
    inline static uint32_t frameNumber = 0;
    inline static std::vector<float> fpsHistory = {};

};

#endif //VULKAN_ENGINE_WINDOW_H
