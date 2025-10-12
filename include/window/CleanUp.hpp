#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL_main.h>

#include <thread>

#include "State.hpp"
void SDL_AppQuit(void *appstate, SDL_AppResult result){
    AppState* as = (AppState*)appstate;

    as->signal_to_exit = true;

    as->eventThread.join();
    
    SDL_DestroyWindow(as->window);
    SDL_free(appstate);
    SDL_Vulkan_UnloadLibrary();
}