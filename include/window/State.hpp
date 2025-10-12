#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>

#include <thread>
typedef struct{
    SDL_Window* window;
    std::thread eventThread;
    std::thread physicsThread;
    bool        signal_to_exit;
} AppState;