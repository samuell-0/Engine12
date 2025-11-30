#define SDL_MAIN_USE_CALLBACKS 1 

#include "ui/ui.h"
#include "renderer/core/Renderer.h"

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    AppState* as = (AppState*)appstate;

    as->signal_to_exit = true;
    // as->physics_thread.join();
    // Ensure the device is idle before destroying any children
    as->disp.deviceWaitIdle();

    UI::shutdown_imgui(as);
    Renderer::clean_up(as);
    UI::clean_up(as);
}