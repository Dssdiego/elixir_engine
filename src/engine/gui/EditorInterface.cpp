//
// Created by Diego S. Seabra on 10/05/22.
//

#include "EditorInterface.h"
#include "../profiling/Logger.h"
#include "../rendering/Window.h"
#include "../rendering/VulkanRenderer.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
CEditorInterfaceImpl* mEditorInterfaceImpl = nullptr;

void CEditorInterface::Init()
{
    mEditorInterfaceImpl = new CEditorInterfaceImpl;
}

void CEditorInterface::Draw()
{
    // TODO: Implement
//    ImGui_ImplVulkan_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();

//    ImGui::ShowDemoWindow();
}

void CEditorInterface::Shutdown()
{
    delete mEditorInterfaceImpl;
}

CEditorInterfaceImpl::CEditorInterfaceImpl()
{
    CLogger::Info("Initializing editor interface");

    InitializeImGui();
}

CEditorInterfaceImpl::~CEditorInterfaceImpl()
{
    CLogger::Info("Shutting down editor interface");

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
    ImGui_ImplGlfw_InitForVulkan(CWindow::GetWindow(), true);

    // Setting ImGui to use dark colors
    ImGui::StyleColorsDark();

    // Setup Vulkan init info
    ImGui_ImplVulkan_InitInfo imguiInfo{};
    imguiInfo.Instance = CVulkanRenderer::GetInstance();
    imguiInfo.PhysicalDevice = CVulkanRenderer::GetPhysicalDevice();
    imguiInfo.Device = CVulkanRenderer::GetLogicalDevice();
    imguiInfo.QueueFamily = 0; // REVIEW: Is the graphics queue always the first one (aka index 0)?
    imguiInfo.Queue = CVulkanRenderer::GetGraphicsQueue();
    imguiInfo.DescriptorPool = CVulkanRenderer::GetDescriptorPool();
    imguiInfo.MinImageCount = CVulkanRenderer::GetSwapChainImageCount();
    imguiInfo.ImageCount = CVulkanRenderer::GetSwapChainImageCount();
    imguiInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    imguiInfo.Allocator = nullptr; // not using an allocator right now

    // Init ImGui for Vulkan
    ImGui_ImplVulkan_Init(&imguiInfo, CVulkanRenderer::GetRenderPass());

    // TODO: Allocate a command buffer
    // TODO: Record the command buffer
    // TODO: Submit to the GPU
}
