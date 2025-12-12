#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <imgui.h>
#include <thread>

#define CLAMP(num, min, max) (num > max ? max : num < min ? min : num)

struct RenderData
{
    VkQueue                         graphics_queue;
    VkQueue                         present_queue;

    std::vector<VkImage>            swapchain_images;//for attachment #1 ie, resolve_attachment
    std::vector<VkImageView>        swapchain_image_views;
    VkImage                         msaa_image;//for attachment #0 ie, color_attachment for msaa...only one needed
    VkImageView                     msaa_image_view;
    VkDeviceMemory                  msaa_image_memory;
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
    size_t current_frame;
};

enum class SettingOpened { None, Test };
struct UIData
{
    uint16_t min_middle_width, min_middle_height;
    uint16_t window_width, window_height;
    uint16_t V0, V1;
    uint16_t H0, H1;
    uint16_t V2, V3;

    SettingOpened which_setting;
};

struct AppState
{
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

    VkDescriptorPool ImGui_desc_pool;

    UIData  ui_data;
    VkClearValue clearColor{{1.0f, 1.0f, 1.0f, 1.0f}};
};