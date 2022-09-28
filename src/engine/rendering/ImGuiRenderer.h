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
    static void DrawObjectInspectorWindow();
    static void DrawProfilerWindow();
    static void DrawCameraWindow();
    static void DrawDebugLogWindow();
    static void DrawAddGameObjectWindow();
    static void DrawWorldEditorWindow();

    // control variables
    inline static bool showCameraWindow = true;
    inline static bool showInspectorWindow = true;
    inline static bool showDebugLogWindow = false;
    inline static bool showAddGameObjectWindow = false;
    inline static bool showProfilerWindow = false;
    inline static bool showWorldEditorWindow = false;
};

#endif //VULKAN_ENGINE_IMGUIRENDERER_H
