#define SDL_MAIN_USE_CALLBACKS 1
#include <VkBootstrap.h>

#include <thread>

#include "ui/ui.h"
#include "renderer/core/Device.h"
#include "renderer/core/Swapchain.h"
#include "renderer/core/Renderer.h"
#include "renderer/pipelines/Pipeline.h"
#include "renderer/core/CommandPool.h"
#include "renderer/utils/ShaderModule.h"

#include "core/Log.hpp"
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){

    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
    
    if(as == nullptr)   return SDL_APP_FAILURE;
    *appstate = as;
    // NOTE: Should impl headless mode.
    if (!UI::create_window(as))                 return SDL_APP_FAILURE;

    if (Device::device_initialization(as)         != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (Swapchain::create_swapchain(as)           != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (Device::get_queues(as)                    != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (Renderer::create_render_pass(as)          != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (Pipeline::create_graphics_pipeline(as)    != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (Swapchain::create_framebuffers(as)        != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (CommandPool::create_command_pool(as)      != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (CommandPool::allocate_command_buffers(as) != VK_SUCCESS)      return SDL_APP_FAILURE;
    if (Renderer::create_sync_objects(as)         != VK_SUCCESS)      return SDL_APP_FAILURE;
// // somewhere after you create your framebuffers / image views
// for (uint32_t i = 0; i < as->swapchain.image_count; i++) {
//     VkImageView view = as->swapchain.get_image_views().value()[i];
//     // If you have access to the create info, or just print the swapchain format again:
//     printf("Swapchain image %d view uses format: %d\n", i, (int)as->swapchain.image_format);
// }
    if (UI::init_imgui(as)                      != VK_SUCCESS)      return SDL_APP_FAILURE;
    return SDL_APP_CONTINUE;
}