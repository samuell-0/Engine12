#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>

#include <thread>

typedef struct {
    VkQueue graphics_queue;
    VkQueue present_queue;

    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;
    std::vector<VkFramebuffer> framebuffers;

    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;

    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffers;

    std::vector<VkSemaphore> available_semaphores;
    std::vector<VkSemaphore> finished_semaphore;
    std::vector<VkFence> in_flight_fences;
    std::vector<VkFence> image_in_flight;
    size_t current_frame = 0;
} RenderData;

typedef struct{
    RenderData  render_data              ;//= NULL;
    bool        signal_to_exit           = false;
    std::thread event_thread             ;//= NULL;
    std::thread physics_thread           ;//= NULL;

    SDL_Window* window                   ;//= NULL;
    VkSurfaceKHR surface                 = VK_NULL_HANDLE;
    vkb::Instance instance               ;//= NULL;
    vkb::InstanceDispatchTable inst_disp ;//= NULL;
    vkb::Device device                   ;//= NULL;
    vkb::DispatchTable disp              ;//= NULL;
    vkb::Swapchain swapchain             ;//= NULL;

    VkDescriptorPool imgui_desc_pool     = VK_NULL_HANDLE;
} AppState;