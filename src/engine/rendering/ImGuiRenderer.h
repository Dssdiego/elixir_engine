//
// Created by Diego S. Seabra on 22/07/22.
//

#ifndef VULKAN_ENGINE_IMGUIRENDERER_H
#define VULKAN_ENGINE_IMGUIRENDERER_H

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
};

#endif //VULKAN_ENGINE_IMGUIRENDERER_H
