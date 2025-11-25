#include "ui/ui.h"
#include "renderer/core/CommandPool.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_sdl3.h" // if using GLFW
#include "iostream"
#include "core/Log.hpp"
bool UI::create_window(AppState* appstate){
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    if (!SDL_Vulkan_LoadLibrary(nullptr)) return -1;
    
    SDL_Window* window = SDL_CreateWindow("vulkan", 1000, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS);
    if (window == nullptr) return -1;

    appstate->window = window;
    int x, y;
    SDL_GetWindowSizeInPixels(window, &x, &y);
    appstate->ui_data.window_width = x;
    appstate->ui_data.window_hight = y;
    return true;
}
void UI::clean_up(AppState* appstate){
    SDL_DestroyWindow(appstate->window);
    SDL_free(appstate);
    SDL_Vulkan_UnloadLibrary();
}
static VkResult create_imgui_descriptor_pool(AppState* appstate){
    VkDescriptorPoolSize pool_sizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets       = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes    = pool_sizes;

    VkResult res = appstate->disp.createDescriptorPool(&pool_info, nullptr, &appstate->imgui_desc_pool);
    if (res != VK_SUCCESS)
        return Log::push(LogLevel::Warning, "unbl to ctr descriptor pool", res);
    return VK_SUCCESS;
}
VkResult UI::init_imgui(AppState* appstate){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // optional
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // optional

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForVulkan(appstate->window))
        return Log::push(LogLevel::Error, "at ImGui_ImplSDL3_InitForVulkan", VK_ERROR_UNKNOWN);

    if (create_imgui_descriptor_pool(appstate) != VK_SUCCESS)
        return Log::push(LogLevel::Error, "unbl to ctr imgui descriptor pool(init_imgui)", VK_ERROR_UNKNOWN);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.ApiVersion = VK_API_VERSION_1_0;
    init_info.Instance        = appstate->instance;
    init_info.PhysicalDevice  = appstate->device.physical_device;
    init_info.Device          = appstate->device;
    init_info.QueueFamily     = appstate->device.get_queue_index(vkb::QueueType::graphics).value();
    init_info.Queue           = appstate->render_data.graphics_queue;
    init_info.PipelineCache   = VK_NULL_HANDLE;
    init_info.DescriptorPool  = appstate->imgui_desc_pool;
    // Provide the renderpass and subpass info so ImGui can create its own
    // pipeline with the correct blending/depth state. If omitted and we
    // accidentally render ImGui with the app pipeline, depth/write state
    // mismatches can hide the triangle.
    init_info.PipelineInfoMain.RenderPass = appstate->render_data.render_pass;
    init_info.PipelineInfoMain.Subpass = 0;
    // Represents the number of samples per pixel used for multisample anti-aliasing (MSAA) in the Vulkan pipeline
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    // init_info.Subpass         = 0;
    init_info.MinImageCount   = appstate->swapchain.image_count;
    init_info.ImageCount      = appstate->swapchain.image_count;
    init_info.Allocator       = nullptr;
    // init_info.CheckVkResultFn = [](VkResult err) { VK_CHECK(err); };
    
    if (!ImGui_ImplVulkan_Init(&init_info))
        return Log::push(LogLevel::Error, "unbl to init vulkan at ImGui_ImplVulkan_Init", VK_ERROR_UNKNOWN);
    
    return VK_SUCCESS;
}
void UI::shutdown_imgui(AppState* appstate){
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
void draw_left_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(12, 12, 15, 255));
    ImGui::BeginChild("left", ImVec2(0, appstate->ui_data.window_hight), 0, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("left");
    for (int i = 0; i < 50; i++) ImGui::Text("Line %d", i);

    ImGui::EndChild();
    ImGui::PopStyleColor();
}
void draw_right_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(12, 12, 15, 255));

    ImGui::BeginChild("right", ImVec2(0, appstate->ui_data.window_hight), 0, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("right");
    for (int i = 0; i < 50; i++) ImGui::Text("Line %d", i);

    ImGui::EndChild();
    ImGui::PopStyleColor();
}
void draw_buttom_panel(AppState* appstate){
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(12, 12, 15, 255));
    ImGui::BeginChild("##BottomPanel", ImVec2(0, 0), 0, ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("BOTTOM PANEL always sticks to the ground");
    for (int i = 0; i < 50; i++) ImGui::Text("Line %d", i);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}
void draw_viewport_window(AppState* appstate){
    ImGui::BeginChild("##TopPanel", ImVec2(0, 0), 0 | ImGuiChildFlags_ResizeY, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
    ImGui::Text("Top panel grows/shrinks freely");
    for (int i = 0; i < 50; i++) ImGui::Text("Line %d", i);
    ImGui::EndChild();
}
void UI::draw_ui(AppState* appstate){
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(appstate->ui_data.window_width, appstate->ui_data.window_hight));

    ImGui::Begin("##FullscreenOverlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    if (ImGui::BeginTable("MainLayout", 3, ImGuiTableFlags_Resizable)){
        ImGui::TableNextRow();

        ImGui::TableNextColumn(); draw_left_panel(appstate);
        ImGui::TableNextColumn(); draw_viewport_window(appstate); draw_buttom_panel(appstate);
        ImGui::TableNextColumn(); draw_right_panel(appstate);
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
    ImGui::End();
    
    ImGui::Render();
}