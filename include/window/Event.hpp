#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL_main.h>

#include <thread>


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    // ImGui_ImplSDL3_ProcessEvent see it!
    // SDL_RegisterEvents()
    // event->user.
    // SDL_PushEvent
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}
