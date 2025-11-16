#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL_main.h>
#include "renderer/core/Renderer.h"

// scene update(the physics theade will handle the computations)
SDL_AppResult SDL_AppIterate(void *appstate){

    bool t = true;
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    if (ImGui::Begin("imgui")){
    //     ImGui::Text("This is some useful text.");
        ImGui::End();
    }
    ImGui::Render();

    if (Renderer::draw_frame((AppState*)appstate) != VK_SUCCESS)
        return SDL_APP_FAILURE;
    return SDL_APP_CONTINUE;
}
