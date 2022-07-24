//
// Created by Diego S. Seabra on 22/07/22.
//

#ifndef VULKAN_ENGINE_IMGUIRENDERER_H
#define VULKAN_ENGINE_IMGUIRENDERER_H

#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

class ImGuiRenderer
{
public:
    ImGuiRenderer();
    ~ImGuiRenderer();

    static void Init();

    void NewFrame();
    void Draw(VkCommandBuffer commandBuffer);

private:
    // we are using a separate descriptor pool for imgui
    static VkDescriptorPool descriptorPool;
};

#endif //VULKAN_ENGINE_IMGUIRENDERER_H
