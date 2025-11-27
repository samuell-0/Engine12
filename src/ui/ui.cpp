#include "ui/ui.h"
#include "renderer/core/CommandPool.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_sdl3.h"
#include "iostream"
#include "core/Log.hpp"
#include "imgui.h"

void set_custom_theming()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.ScrollbarRounding   = 2.0f;
    style.ScrollbarSize = 6.0f;
    style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);

    style.Colors[ImGuiCol_Text]                   =  ImVec4(0.40f, 0.63f, 0.87f, 1.00f);

    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(10.0f / 255, 10.0f / 255, 10.0f / 255, 1.00f);
    style.Colors[ImGuiCol_TableBorderLight]       = ImVec4(10.0f / 255, 10.0f / 255, 10.0f / 255, 1.00f);

    style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

    style.Colors[ImGuiCol_Tab]                    = ImVec4(16.0f / 255, 28.0f / 255, 33.0f / 255, 255);
    style.Colors[ImGuiCol_TabSelected]            = ImVec4(16.0f / 255, 28.0f / 255, 33.0f / 255, 255);
    style.Colors[ImGuiCol_TabSelectedOverline]    = ImVec4(16.0f / 255, 28.0f / 255, 33.0f / 255, 255);
    style.Colors[ImGuiCol_TabActive]              = ImVec4(16.0f / 255, 28.0f / 255, 33.0f / 255, 255);
    style.Colors[ImGuiCol_TabHovered]              = ImVec4(16.0f / 255, 28.0f / 255, 33.0f / 255, 255);

    style.CellPadding     = ImVec2(0.0f, 0.0f);
    style.ItemSpacing     = ImVec2(0.0f, 0.0f);
    style.FrameBorderSize = 10.0f;
    style.TabRounding     = 1.0f;
    style.TabMinWidthBase = 70.0f;
}

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
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0,0,0,1);
    style.Colors[ImGuiCol_ChildBg]  = ImVec4(0,0,0,0);
    style.Alpha = 1.0f;
    ImGui::StyleColorsDark();
    set_custom_theming();

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
    init_info.PipelineInfoMain.Subpass    = 0;
    // Represents the number of samples per pixel used for multisample anti-aliasing (MSAA) in the Vulkan pipeline
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_4_BIT;
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
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
    ImGui::BeginChild("left", ImVec2(0, appstate->ui_data.window_hight));
    if (ImGui::BeginTabBar("rsv")){
        if (ImGui::BeginTabItem("sac") ){
            for (int i = 0; i < 100; i++) ImGui::Text(" ");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("sadccCSEFc") ){
            for (int i = 0; i < 100; i++) ImGui::Text(" ");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("savdrjfnksdllc") ){
            for (int i = 0; i < 100; i++) ImGui::Text(" ");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void draw_plot_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));

    ImGui::BeginChild("##plot", ImVec2(0, 0), ImGuiChildFlags_ResizeY);
    for (int i = 0; i < 100; i++) ImGui::Text(" ");
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

void draw_editor_panel(AppState* appstate){
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));

    ImGui::BeginChild("##editor", ImVec2(0, 0));
    for (int i = 0; i < 100; i++) ImGui::Text(" ");
    ImGui::EndChild();

    ImGui::PopStyleColor();
}

void draw_buttom_panel(AppState* appstate){
    
    if (ImGui::BeginChild("##BottomPanel", ImVec2(0, 0))){
        if (ImGui::BeginTable("fvj", 2, ImGuiTableFlags_Resizable)){

            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(16, 28, 33, 255));
            if (ImGui::BeginChild("leftkjdzc", ImVec2(0, 0))){
                for (int i = 0; i < 100; i++) ImGui::Text(" ");
                ImGui::EndChild();
            }
            ImGui::PopStyleColor();

            ImGui::TableNextColumn();
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(10, 10, 10, 255));
            if (ImGui::BeginChild("rightkjdzc", ImVec2(0, 0))){
                for (int i = 0; i < 100; i++) ImGui::Text(" ");
                ImGui::EndChild();
            }
            ImGui::PopStyleColor();

            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
}

void draw_viewport_window(AppState* appstate){
    ImGui::BeginChild("##TopPanel", ImVec2(0, 0), ImGuiChildFlags_ResizeY, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoScrollbar);
    for (int i = 0; i < 100; i++) ImGui::Text(" ");
    ImGui::EndChild();
}

void draw_menu_bar_m(AppState* appstate){
    ImGui::BeginChild("LeftPanel", ImVec2(0, 1), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
    if (ImGui::BeginMenuBar()){
        float width = ImGui::GetWindowWidth();
        ImGui::MenuItem("New"); ImGui::Separator();
        ImGui::MenuItem("Open"); ImGui::Separator();
        ImGui::MenuItem("Save"); ImGui::Separator();
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Copy");
            ImGui::MenuItem("Paste");
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Recompile Shaders");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void draw_menu_bar_r(AppState* appstate){
    ImGui::BeginChild("LeftfgPanel", ImVec2(0, 1), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
    if (ImGui::BeginMenuBar()){
        float width = ImGui::GetWindowWidth();
        ImGui::MenuItem("New"); ImGui::Separator();
        ImGui::MenuItem("Open"); ImGui::Separator();
        ImGui::MenuItem("Save"); ImGui::Separator();
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Copy");
            ImGui::MenuItem("Paste");
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Recompile Shaders");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void UI::draw_ui(AppState* appstate){

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(appstate->ui_data.window_width, appstate->ui_data.window_hight));
    ImGui::Begin("##FullscreenOverlay", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    // ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(100, 175, 203, 255));
    if (ImGui::BeginTable("MainLayout", 3, ImGuiTableFlags_Resizable)){
        ImGui::TableNextRow();

        ImGui::TableNextColumn(); draw_left_panel(appstate);
        ImGui::TableNextColumn(); draw_menu_bar_m(appstate); draw_viewport_window(appstate); draw_buttom_panel(appstate);
        ImGui::TableNextColumn(); draw_menu_bar_r(appstate); draw_plot_panel(appstate); draw_editor_panel(appstate);
        ImGui::EndTable();
    }
    ImGui::End();

    // ImGui::PopStyleVar();
    ImGui::Render();
}