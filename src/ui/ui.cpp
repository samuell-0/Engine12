#include "ui/ui.h"
#include "renderer/core/CommandPool.h"
#include "core/Log.hpp"
ImVec4 vec4(float red, float green, float blue, float alpha){
    return ImVec4(red / 255, green / 255, blue / 255, alpha / 255);
}
void set_custom_theming(){
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.ButtonTextAlign = ImVec2(0.0f, 0.5f);
    style.CellPadding     = ImVec2(0.0f, 0.0f);
    style.ItemSpacing     = ImVec2(0.0f, 0.0f);

    style.TabRounding     = 1.0f;
    style.TabMinWidthBase = 70.0f;
    // style.AntiAliasedFill = true;// NOTE: GPU
    // style.ChildRounding   = 10.0f;
    style.ScrollbarRounding = 2.0f;
    style.ScrollbarSize     = 6.0f;

    style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);
    // style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.40f, 0.63f, 0.87f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]            = vec4(10, 10, 10, 255);

    style.Colors[ImGuiCol_Text]                 = vec4(160, 215, 237, 255);

    style.Colors[ImGuiCol_TableBorderLight]     = vec4(10, 10, 10, 255);

    style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    style.Colors[ImGuiCol_Tab]                  = vec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_TabSelected]          = vec4(100, 100, 150, 100);
    style.Colors[ImGuiCol_TabSelectedOverline]  = vec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_TabHovered]           = vec4(100, 100, 150, 150);

    style.Colors[ImGuiCol_Button]               = vec4(16, 28, 33, 0);
    style.Colors[ImGuiCol_ButtonActive]         = vec4(100, 100, 150, 76);
    style.Colors[ImGuiCol_ButtonHovered]        = vec4(100, 100, 150, 130);

}

bool UI::create_window(AppState* appstate){
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    if (!SDL_Vulkan_LoadLibrary(nullptr)) return -1;
    
    SDL_Window* window = SDL_CreateWindow("vulkan", 1000, 600, SDL_WINDOW_VULKAN);
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
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;
    config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("/home/Sam/Projects/Engine12/resources/Roboto_Condensed-Italic.ttf", 14.0f, &config);

    static const ImWchar icon_ranges[] = { 0xf000, 0xf8ff, 0 }; // FontAwesome range
    ImFontConfig config_icons;
    config_icons.MergeMode = true;           // <<-- important: merge into previous font
    config_icons.PixelSnapH = true;
    config_icons.GlyphMinAdvanceX = 20.0f;   // optional: make icons same width
    io.Fonts->AddFontFromFileTTF("/home/Sam/Projects/Engine12/resources/fa-solid-900.ttf", 18.0f, &config_icons, icon_ranges);

    ImGui::StyleColorsClassic();
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

