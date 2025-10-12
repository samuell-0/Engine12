#define SDL_MAIN_USE_CALLBACKS 1 
#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

#include <thread>

#include "window/descriptor.hpp"
#include "physics/PhysicsThread.h"
#include "State.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){

    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));

    if(as == nullptr)
        return SDL_APP_FAILURE;

    if (!SDL_Init(SDL_INIT_VIDEO))
        return SDL_APP_FAILURE;

    if (!SDL_Vulkan_LoadLibrary(nullptr))
        return SDL_APP_FAILURE;
    
    as->window = SDL_CreateWindow("vulkan", 800, 600, SDL_WINDOW_VULKAN|SDL_WINDOW_RESIZABLE);
    if (as == nullptr)
        return SDL_APP_FAILURE;

    *appstate = as;

    as->eventThread   = std::thread(watchFiles, as);
    
    return SDL_APP_CONTINUE;
}