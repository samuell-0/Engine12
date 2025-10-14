#define SDL_MAIN_USE_CALLBACKS 1 
#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>

#include <thread>

#include "window/descriptor.hpp"
#include "physics/PhysicsThread.h"
#include "State.hpp"

#include "renderer/core/Renderer.h"
#include "renderer/core/Device.h"
#include "renderer/core/Swapchain.h"
#include "renderer/pipelines/Pipeline.h"
#include "renderer/utils/ShaderModule.h"
#include "renderer/core/CommandPool.h"

bool create_window(AppState* appstate){
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    if (!SDL_Vulkan_LoadLibrary(nullptr)) return -1;
    
    SDL_Window* window = SDL_CreateWindow("vulkan", 800, 600, SDL_WINDOW_VULKAN|SDL_WINDOW_RESIZABLE);
    if (window == nullptr) return -1;

    appstate->window = window;
    return true;
}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){

    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
    if(as == nullptr)
        return SDL_APP_FAILURE;
    *appstate = as;
    SDL_APP_FAILURE;
    if (!create_window(as))
        return SDL_APP_FAILURE;
    if (!Device::device_initialization(as))
        return SDL_APP_FAILURE;
    if (!Swapchain::create_swapchain(as))
        return SDL_APP_FAILURE;
    if (!Device::get_queues(as))
        return SDL_APP_FAILURE;
    if (!Renderer::create_render_pass(as))
        return SDL_APP_FAILURE;
    if (!Pipeline::create_graphics_pipeline(as))
        return SDL_APP_FAILURE;
    if (!Swapchain::create_framebuffers(as))
        return SDL_APP_FAILURE;
    if (!CommandPool::create_command_pool(as))
        return SDL_APP_FAILURE;
    if (!CommandPool::create_command_buffers(as))
        return SDL_APP_FAILURE;
    if (!Renderer::create_sync_objects(as))
        return SDL_APP_FAILURE;
    return SDL_APP_CONTINUE;
}