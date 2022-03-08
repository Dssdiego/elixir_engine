//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef VULKAN_ENGINE_VULKANENGINE_H
#define VULKAN_ENGINE_VULKANENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

struct EngineConfig
{
    std::string title;
    VkExtent2D windowSize;
    VkExtent2D minWindowSize;
};

class CVulkanEngine
{
public:
    void Init(EngineConfig* pConfig); // initializes everything in the engine
    void Run();  // main loop
    void Draw(); // draw loop
    void Cleanup(); // shuts down the engine

private:
    bool mIsInitialized = false;
    int mFrameNumber = 0;

    VkExtent2D mWindowSize;
    VkExtent2D mMinWindowSize;
    std::string mWindowTitle;
    GLFWwindow* mWindow = nullptr;
};


#endif //VULKAN_ENGINE_VULKANENGINE_H
