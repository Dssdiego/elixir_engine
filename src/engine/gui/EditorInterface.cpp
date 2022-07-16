//
// Created by Diego S. Seabra on 10/05/22.
//

#include <Tracy.hpp>
#include "EditorInterface.h"
#include "../profiling/Logger.h"
#include "../rendering/Window.h"
#include "../rendering/VulkanContext.h"
#include "../rendering/VulkanRenderer.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
CEditorInterfaceImpl* mEditorInterfaceImpl = nullptr;

void EditorInterface::Init()
{
    mEditorInterfaceImpl = new CEditorInterfaceImpl;
}

void EditorInterface::Draw()
{
    ZoneScopedC(0x34495e);
    // TODO: Implement
//    ImGui_ImplVulkan_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();

//    ImGui::ShowDemoWindow();
}

void EditorInterface::Shutdown()
{
    delete mEditorInterfaceImpl;
}

CEditorInterfaceImpl::CEditorInterfaceImpl()
{
    Logger::Info("Initializing editor interface");

    InitializeImGui();
}

CEditorInterfaceImpl::~CEditorInterfaceImpl()
{
    Logger::Info("Shutting down editor interface");

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void CEditorInterfaceImpl::InitializeImGui()
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
    imguiInfo.Instance = vkContext.instance;
    imguiInfo.PhysicalDevice = vkContext.physicalDevice;
    imguiInfo.Device = vkContext.logicalDevice;
    imguiInfo.QueueFamily = vkContext.graphicsFamilyIdx.value();
    imguiInfo.Queue = vkContext.graphicsQueue;
    imguiInfo.DescriptorPool = vkContext.descriptorPool;
    imguiInfo.MinImageCount = vkContext.swapChainImageCount;
    imguiInfo.ImageCount = vkContext.swapChainImageCount;
    imguiInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    imguiInfo.Allocator = nullptr; // not using an allocator right now

    // Init ImGui for Vulkan
    ImGui_ImplVulkan_Init(&imguiInfo, vkContext.renderPass);

    // create fonts texture in the GPU
    VkCommandBuffer commandBuffer = CVulkanRenderer::BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    CVulkanRenderer::EndSingleTimeCommands(commandBuffer);

    Logger::Debug("Initialized imgui for vulkan");

    // TODO: Allocate a command buffer
    // TODO: Record the command buffer
    // TODO: Submit to the GPU
}
