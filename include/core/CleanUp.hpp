#define SDL_MAIN_USE_CALLBACKS 1 

#include "ui/ui.h"
#include "renderer/core/Renderer.h"

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    AppState* as = (AppState*)appstate;

    as->signal_to_exit = true;

    // Renderer::clean_up(as);

    // UI::clean_up(as);
}