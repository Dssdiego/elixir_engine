//
// Created by Diego S. Seabra on 22/07/22.
//

#include "ImGuiRenderer.h"
#include "vulkan/VulkanContext.h"
#include "Window.h"
#include "vulkan/VulkanDevice.h"
#include "vulkan/VulkanSwapchain.h"
#include "vulkan/VulkanEngine.h"
#include "TestRenderSystem.h"
#include "vulkan/VulkanPipeline.h"
#include "Camera.h"

// checking for vulkan error
static void check_vk_result(VkResult err) {
    if (err == 0) return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0) abort();
}

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
ImGuiRendererImpl* mImGuiRendererImpl = nullptr;

//
// Initialization/Destruction
//

void ImGuiRenderer::Init()
{
#ifndef NDEBUG
    mImGuiRendererImpl = new ImGuiRendererImpl;
#endif
}

void ImGuiRenderer::Shutdown()
{
#ifndef NDEBUG
    delete mImGuiRendererImpl;
#endif
}

//
// External
//

void ImGuiRenderer::NewFrame()
{
#ifndef NDEBUG
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
#endif
}

void ImGuiRenderer::Draw()
{
    bool show_demo = true;
    ImGui::ShowDemoWindow(&show_demo);

    DrawMainMenuBar();

    if (showInspectorWindow)
        DrawObjectInspectorWindow();

    if (showCameraWindow)
        DrawCameraWindow();
}

//
// Draw Methods
//
void ImGuiRenderer::DrawMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        // Engine
        if (ImGui::BeginMenu("Engine"))
        {
            // Engine >> Exit
            if (ImGui::MenuItem("Exit", "CTRL+Q")) { Window::Close(); }

            ImGui::EndMenu();
        }

        // Game
        if (ImGui::BeginMenu("Game"))
        {
            // Game >> Inspector
            if (ImGui::MenuItem("Game Object Inspector", nullptr, &showInspectorWindow)) {}
//            ImGui::EndMenu();

            // Game >> Camera
            if (ImGui::MenuItem("Camera", nullptr, &showCameraWindow)) {}

            ImGui::EndMenu();
        }

//        if (ImGui::BeginMenu("Edit"))
//        {
//            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
//            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
//            ImGui::Separator();
//            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
//            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
//            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
//            ImGui::EndMenu();
//        }
        ImGui::EndMainMenuBar();
    }
}

void ImGuiRenderer::DrawObjectInspectorWindow()
{
    if (ImGui::Begin("Inspector", &showInspectorWindow))
    {
        if (ImGui::TreeNode("GameObjects"))
        {
            for (auto &obj: *TestRenderSystem::GetGameObjects())
            {
                if (ImGui::TreeNode((void *) (intptr_t) obj.id, "[%d] %s", obj.id, obj.name.c_str()))
                {
                    ImGui::Text("Pipeline: %s", obj.pipeline.c_str());
                    ImGui::InputFloat3("position", (float *) &obj.transform.position);
                    ImGui::InputFloat("rotation", &obj.transform.rotation, 0.1f, 1.0f, "%.3f");
                    ImGui::InputFloat3("scale", (float *) &obj.transform.scale);
                    ImGui::ColorEdit4("color", (float *) &obj.color);

                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::End();
    }
}

void ImGuiRenderer::DrawCameraWindow()
{
    if (ImGui::Begin("Camera", &showCameraWindow))
    {
        ImGui::Checkbox("Control Camera?", &Camera::takeControl);

        ImGui::InputFloat3("position", (float *) Camera::GetWorldPositionRef());
        if (!Camera::HasControl())
            Camera::SetWorldPosition(Camera::GetWorldPosition());

        if (ImGui::Button("Reset"))
            Camera::ResetWorldPosition();

        ImGui::End();
    }
}

void ImGuiRenderer::Render()
{
#ifndef NDEBUG
    Draw();
    ImGui::Render();
    ImDrawData *drawdata = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawdata, VulkanEngine::GetCurrentCommandBuffer());
#endif
}

//
// Implementation
//

ImGuiRendererImpl::ImGuiRendererImpl()
{
    Logger::Debug("Initializing ImGuiRenderer");

    CreateDescriptorPool();
    SetupContext();
    BindVulkanBackend();
}

ImGuiRendererImpl::~ImGuiRendererImpl()
{
    Logger::Debug("Destroying ImGuiRenderer");

    vkDeviceWaitIdle(VulkanDevice::GetDevice());

    vkDestroyDescriptorPool(VulkanDevice::GetDevice(), descriptorPool, nullptr);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiRendererImpl::CreateDescriptorPool()
{
    VkDescriptorPoolSize pool_sizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    VK_CHECK(vkCreateDescriptorPool(VulkanDevice::GetDevice(), &pool_info, nullptr, &descriptorPool));
}

void ImGuiRendererImpl::SetupContext()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void ImGuiRendererImpl::BindVulkanBackend()
{
    // Setup Platform/Renderer backends
    // Initialize imgui for vulkan
    ImGui_ImplGlfw_InitForVulkan(Window::GetWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = VulkanDevice::GetInstance();
    init_info.PhysicalDevice = VulkanDevice::GetPhysicalDevice();
    init_info.Device = VulkanDevice::GetDevice();
    init_info.QueueFamily = VulkanDevice::GetGraphicsQueueFamilyIdx();
    init_info.Queue = VulkanDevice::GetGraphicsQueue();

    // pipeline cache is a potential future optimization, ignoring for now
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptorPool;
    init_info.Allocator = VK_NULL_HANDLE; // not using an allocator right now
    init_info.MinImageCount = 2;
    init_info.ImageCount = VulkanSwapchain::GetImageCount();
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, VulkanSwapchain::GetRenderPass());

    // upload fonts, this is done by recording and submitting a one time use command buffer
    // which can be done easily bye using some existing helper functions on the lve device object
    auto commandBuffer = VulkanDevice::BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    VulkanDevice::EndSingleTimeCommands(commandBuffer);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}
