#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <imgui.h>
#include <thread>

typedef struct {
    VkQueue                         graphics_queue;
    VkQueue                         present_queue;

    std::vector<VkImage>            swapchain_images;//for attachment #1 ie, resolve_attachment
    std::vector<VkImageView>        swapchain_image_views;
    VkImage                         msaa_image;//for attachment #0 ie, color_attachment for msaa...only one needed
    VkImageView                     msaa_image_view;
    VkDeviceMemory                  msaa_memory;
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

enum class SettingOpened { None, Test };
typedef struct{
    int window_width;
    int window_hight;

    SettingOpened which_setting;
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
    VkClearValue clearColor{{1.0f, 1.0f, 1.0f, 1.0f}};
} AppState;