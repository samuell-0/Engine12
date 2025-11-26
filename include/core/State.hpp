#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <imgui.h>
#include <thread>

typedef struct {
    VkQueue                         graphics_queue;
    VkQueue                         present_queue;

    std::vector<VkImage>            swapchain_images;
    std::vector<VkImageView>        swapchain_image_views;
    std::vector<VkFramebuffer>      framebuffers;

    VkRenderPass                    render_pass;
    VkPipelineLayout                pipeline_layout;
    VkPipeline                      graphics_pipeline;

    VkCommandPool command_pool;
    std::vector<VkCommandBuffer>    command_buffers;

    std::vector<VkSemaphore>        available_semaphores;
    std::vector<VkSemaphore>        finished_semaphore;
    std::vector<VkFence>            in_flight_fences;
    std::vector<VkFence>            image_in_flight;
    size_t current_frame = 0;
} RenderData;
typedef struct{
    float window_width;
    float window_hight;

    ImU32 ui_bg_color;
} UiData;
typedef struct{
    RenderData  render_data;
    bool        signal_to_exit;
    std::thread event_thread;
    std::thread physics_thread;

    SDL_Window*                 window;
    VkSurfaceKHR                surface;
    vkb::Instance               instance;
    vkb::InstanceDispatchTable  inst_disp;
    vkb::Device                 device;
    vkb::DispatchTable          disp;
    vkb::Swapchain              swapchain;

    VkDescriptorPool imgui_desc_pool;

    UiData  ui_data;    
} AppState;