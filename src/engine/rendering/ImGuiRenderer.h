//
// Created by Diego S. Seabra on 22/07/22.
//

#ifndef VULKAN_ENGINE_IMGUIRENDERER_H
#define VULKAN_ENGINE_IMGUIRENDERER_H

#include <glm/gtc/type_ptr.hpp>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

struct ImGuiRendererImpl
{
    ImGuiRendererImpl();
    ~ImGuiRendererImpl();

    // we are using a separate descriptor pool for imgui
    VkDescriptorPool descriptorPool = nullptr;

    void CreateDescriptorPool();
    void SetupContext();
    void BindVulkanBackend();
};

class ImGuiRenderer
{
public:
    static void Init();
    static void Shutdown();

    static void NewFrame();
    static void Render(); // all that is drawed now must be rendered on the screen

private:
    static void Draw(); // here comes all the things we want to draw with ImGui

    static void DrawMainMenuBar();
    static void DrawGameWindow();
    static void DrawObjectInspectorWindow();
    static void DrawCameraWindow();

    // control variables
    inline static bool showCameraWindow = false;
    inline static bool showInspectorWindow = false;
};

#endif //VULKAN_ENGINE_IMGUIRENDERER_H
