#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL_main.h>

#include <thread>


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    // ImGui_ImplSDL3_ProcessEvent see it!
    // SDL_RegisterEvents()
    // event->user.
    // SDL_PushEvent
    ImGui_ImplSDL3_ProcessEvent(event);
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}
