//
// Created by Diego S. Seabra on 22/07/22.
//

#include "ImGuiRenderer.h"
#include "VulkanContext.h"
#include "Window.h"
#include "VulkanDevice.h"

// checking for vulkan error
static void check_vk_result(VkResult err) {
    if (err == 0) return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0) abort();
}

void ImGuiRenderer::Init()
{
    // Check Version and create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui_ImplGlfw_InitForVulkan(Window::GetWindow(), true);

    // Setting ImGui to use dark colors
    ImGui::StyleColorsDark();

    // Setup Vulkan init info
    ImGui_ImplVulkan_InitInfo imguiInfo{};
    imguiInfo.Instance = VulkanDevice::GetInstance();
    imguiInfo.PhysicalDevice = VulkanDevice::GetPhysicalDevice();
    imguiInfo.Device = VulkanDevice::GetDevice();
    imguiInfo.QueueFamily = VulkanDevice::GetGraphicsQueueFamilyIdx();
    imguiInfo.Queue = VulkanDevice::GetGraphicsQueue();
    imguiInfo.MinImageCount = VulkanSwapchain::GetImageCount();
    imguiInfo.ImageCount = VulkanSwapchain::GetImageCount();
    imguiInfo.DescriptorPool = descriptorPool;
    imguiInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    imguiInfo.PipelineCache = VK_NULL_HANDLE; // not using pipeline cache right now
    imguiInfo.Allocator = VK_NULL_HANDLE; // not using an allocator right now
    imguiInfo.CheckVkResultFn = check_vk_result;

    // Init ImGui for Vulkan
    ImGui_ImplVulkan_Init(&imguiInfo, vkContext.renderPass);

    // upload fonts to the GPU. This is done by recording
    // TODO: Implement
    VkCommandBuffer commandBuffer = VulkanDevice::BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    VulkanDevice::EndSingleTimeCommands(commandBuffer);
//    ImGui_ImplVulkan_DestroyFontUploadObjects();

//    Logger::Debug("Initialized imgui for vulkan");
}
