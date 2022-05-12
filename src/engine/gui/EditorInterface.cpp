//
// Created by Diego S. Seabra on 10/05/22.
//

#include "EditorInterface.h"
#include "../profiling/Logger.h"
#include "../rendering/Window.h"
#include "../rendering/VulkanRenderer.h"

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
CEditorInterfaceImpl* mImplementation = nullptr;

void CEditorInterface::Init()
{
    mImplementation = new CEditorInterfaceImpl;
}

void CEditorInterface::Draw()
{

}

void CEditorInterface::Shutdown()
{
    delete mImplementation;
}

CEditorInterfaceImpl::CEditorInterfaceImpl()
{
    CLogger::Info("Initializing editor interface");

    CreateDescriptorPool();
    InitializeImGui();
}

CEditorInterfaceImpl::~CEditorInterfaceImpl()
{
    CLogger::Info("Shutting down editor interface");

    vkDestroyDescriptorPool(CVulkanRenderer::GetLogicalDevice(), imguiPool, nullptr);

    // TODO: Enable imgui-vulkan binding shutdown
//    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void CEditorInterfaceImpl::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = std::size(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(CVulkanRenderer::GetLogicalDevice(), &poolInfo, nullptr, &imguiPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to create ImGui descriptor pool");
    }
}

void CEditorInterfaceImpl::InitializeImGui()
{
    // Create ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui_ImplGlfw_InitForVulkan(CWindow::GetWindow(), true);

    // Setup Vulkan init info
    ImGui_ImplVulkan_InitInfo imguiInfo{};
    imguiInfo.Instance = CVulkanRenderer::GetInstance();
    imguiInfo.PhysicalDevice = CVulkanRenderer::GetPhysicalDevice();
    imguiInfo.Device = CVulkanRenderer::GetLogicalDevice();
    imguiInfo.Queue = CVulkanRenderer::GetGraphicsQueue();
    imguiInfo.DescriptorPool = imguiPool;
    imguiInfo.MinImageCount = 3;
    imguiInfo.ImageCount = 3;
    imguiInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    imguiInfo.Allocator = nullptr; // not using an allocator right now

    // Init ImGui for Vulkan
    // TODO: Implement
//    ImGui_ImplVulkan_Init(&imguiInfo, CVulkanRenderer::GetRenderPass());
}
