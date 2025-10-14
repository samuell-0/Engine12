#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL_main.h>

#include <thread>

#include "State.hpp"

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    AppState* as = (AppState*)appstate;
    
    as->signal_to_exit = true;
    
    for (size_t i = 0; i < as->swapchain.image_count; i++) {
        as->disp.destroySemaphore(as->render_data.finished_semaphore[i], nullptr);
    }
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        as->disp.destroySemaphore(as->render_data.available_semaphores[i], nullptr);
        as->disp.destroyFence(as->render_data.in_flight_fences[i], nullptr);
    }

    as->disp.destroyCommandPool(as->render_data.command_pool, nullptr);

    for (auto framebuffer : as->render_data.framebuffers) {
        as->disp.destroyFramebuffer(framebuffer, nullptr);
    }

    as->disp.destroyPipeline(as->render_data.graphics_pipeline, nullptr);
    as->disp.destroyPipelineLayout(as->render_data.pipeline_layout, nullptr);
    as->disp.destroyRenderPass(as->render_data.render_pass, nullptr);

    as->swapchain.destroy_image_views(as->render_data.swapchain_image_views);

    vkb::destroy_swapchain(as->swapchain);
    vkb::destroy_device(as->device);
    vkb::destroy_surface(as->instance, as->surface);
    vkb::destroy_instance(as->instance);

    SDL_DestroyWindow(as->window);
    SDL_free(appstate);
    SDL_Vulkan_UnloadLibrary();
}